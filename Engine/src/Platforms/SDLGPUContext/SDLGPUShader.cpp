
#include <pch.h>
#include "Platforms/SDLGPU/SDLGPUShader.h"
#include "Platforms/SDLGPU/SDLGPUVertexArray.h"
#include "Platforms/SDLGPU/SDLGPUBuffer.h"
#include "Platforms/SDLGPU/SDLGPURenderAPI.h"
#include "Core/PathService.h"

#include <SDL3/SDL_gpu.h>

#include <fstream>
#include <regex>

namespace Cober {

    const SDLGPUShader* SDLGPUShader::s_BoundShader = nullptr;

    namespace {

        static SDL_GPUDevice* GetDevice()
        {
            auto* api = SDLGPURenderAPI::Get();
            LOG_CORE_ASSERT(api, "SDLGPURenderAPI has not been initialized yet");
            return api->GetDevice();
        }

        static SDL_GPUVertexElementFormat ToVertexFormat(ShaderDataType type)
        {
            switch (type)
            {
                case ShaderDataType::Float:  return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT;
                case ShaderDataType::Float2: return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
                case ShaderDataType::Float3: return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
                case ShaderDataType::Float4: return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
                case ShaderDataType::Int:    return SDL_GPU_VERTEXELEMENTFORMAT_INT;
                case ShaderDataType::Int2:   return SDL_GPU_VERTEXELEMENTFORMAT_INT2;
                case ShaderDataType::Int3:   return SDL_GPU_VERTEXELEMENTFORMAT_INT3;
                case ShaderDataType::Int4:   return SDL_GPU_VERTEXELEMENTFORMAT_INT4;
                default: break;
            }

            LOG_CORE_ASSERT(false, "Unsupported vertex attribute type in SDL_GPU pipeline");
            return SDL_GPU_VERTEXELEMENTFORMAT_INVALID;
        }
    }

    SDLGPUShader::SDLGPUShader(const std::string& filepath)
    {
        const std::filesystem::path glslPath = PathService::ResolveAsset(std::filesystem::path("shaders") / filepath);
        m_Name = glslPath.stem().string();
        LoadFromCombinedShader(glslPath);
    }

    SDLGPUShader::SDLGPUShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
        : m_Name(name)
    {
        (void)vertexSrc;
        (void)fragmentSrc;
        LOG_CORE_ASSERT(false, "Runtime source-based SDL_GPU shader creation is not implemented. Use compiled stage binaries.");
    }

    SDLGPUShader::~SDLGPUShader()
    {
        SDL_GPUDevice* device = SDLGPURenderAPI::Get() ? SDLGPURenderAPI::Get()->GetDevice() : nullptr;
        if (!device)
        {
            return;
        }

        for (auto& [_, pipeline] : m_PipelineCache)
        {
            SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
        }
        m_PipelineCache.clear();

        if (m_VertexShader)
        {
            SDL_ReleaseGPUShader(device, m_VertexShader);
            m_VertexShader = nullptr;
        }

        if (m_FragmentShader)
        {
            SDL_ReleaseGPUShader(device, m_FragmentShader);
            m_FragmentShader = nullptr;
        }
    }

    void SDLGPUShader::Bind() const
    {
        s_BoundShader = this;
    }

    SDL_GPUGraphicsPipeline* SDLGPUShader::GetOrCreatePipeline(const SDLGPUVertexArray& vertexArray, const SDLGPUShaderPassSignature& signature)
    {
        const std::string key = MakePipelineKey(vertexArray, signature);
        auto it = m_PipelineCache.find(key);
        if (it != m_PipelineCache.end())
        {
            return it->second;
        }

        std::vector<SDL_GPUVertexBufferDescription> vbDescriptions;
        std::vector<SDL_GPUVertexAttribute> attributes;

        uint32_t location = 0;
        uint32_t bufferSlot = 0;
        for (const auto& vb : vertexArray.GetVertexBuffers())
        {
            auto sdlvb = std::dynamic_pointer_cast<SDLGPUVertexBuffer>(vb);
            LOG_CORE_ASSERT(sdlvb, "Expected SDLGPUVertexBuffer");

            vbDescriptions.push_back(
                SDL_GPUVertexBufferDescription{
                    .slot = bufferSlot,
                    .pitch = sdlvb->GetLayout().GetStride(),
                    .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                    .instance_step_rate = 0
                }
            );

            for (const auto& element : sdlvb->GetLayout())
            {
                attributes.push_back(
                    SDL_GPUVertexAttribute{
                        .location = location++,
                        .buffer_slot = bufferSlot,
                        .format = ToVertexFormat(element.type),
                        .offset = (uint32_t)element.offset
                    }
                );
            }

            ++bufferSlot;
        }

        std::array<SDL_GPUColorTargetDescription, 4> colorDescs{};
        for (uint32_t i = 0; i < signature.NumColorTargets; ++i)
        {
            colorDescs[i].format = static_cast<SDL_GPUTextureFormat>(signature.ColorFormats[i]);
            colorDescs[i].blend_state.enable_blend = signature.AlphaBlend && i == 0;
            colorDescs[i].blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
            colorDescs[i].blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
            colorDescs[i].blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
            colorDescs[i].blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
            colorDescs[i].blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
            colorDescs[i].blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
            colorDescs[i].blend_state.enable_color_write_mask = false;
        }

        SDL_GPUGraphicsPipelineCreateInfo createInfo = {};
        createInfo.vertex_shader = m_VertexShader;
        createInfo.fragment_shader = m_FragmentShader;
        createInfo.vertex_input_state = SDL_GPUVertexInputState{
            .vertex_buffer_descriptions = vbDescriptions.data(),
            .num_vertex_buffers = (uint32_t)vbDescriptions.size(),
            .vertex_attributes = attributes.data(),
            .num_vertex_attributes = (uint32_t)attributes.size()
        };
        createInfo.primitive_type = static_cast<SDL_GPUPrimitiveType>(signature.PrimitiveType);
        createInfo.rasterizer_state = SDL_GPURasterizerState{
            .fill_mode = SDL_GPU_FILLMODE_FILL,
            .cull_mode = SDL_GPU_CULLMODE_NONE,
            .front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE,
            .depth_bias_constant_factor = 0.0f,
            .depth_bias_clamp = 0.0f,
            .depth_bias_slope_factor = 0.0f,
            .enable_depth_bias = false,
            .enable_depth_clip = true
        };
        createInfo.multisample_state = SDL_GPUMultisampleState{
            .sample_count = (SDL_GPUSampleCount)signature.Samples,
            .sample_mask = 0xFFFFFFFFu,
            .enable_mask = false
        };
        createInfo.depth_stencil_state = SDL_GPUDepthStencilState{
            .compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL,
            .back_stencil_state = {},
            .front_stencil_state = {},
            .compare_mask = 0xFF,
            .write_mask = 0xFF,
            .enable_depth_test = signature.HasDepth,
            .enable_depth_write = signature.HasDepth,
            .enable_stencil_test = false
        };
        createInfo.target_info = SDL_GPUGraphicsPipelineTargetInfo{
            .color_target_descriptions = colorDescs.data(),
            .num_color_targets = signature.NumColorTargets,
            .depth_stencil_format = static_cast<SDL_GPUTextureFormat>(signature.DepthFormat),
            .has_depth_stencil_target = signature.HasDepth
        };

        SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(GetDevice(), &createInfo);
        LOG_CORE_ASSERT(pipeline, "SDL_CreateGPUGraphicsPipeline failed for shader {0}: {1}", m_Name, SDL_GetError());

        m_PipelineCache[key] = pipeline;
        return pipeline;
    }

    const SDLGPUShader* SDLGPUShader::GetBoundShader()
    {
        return s_BoundShader;
    }

    std::unordered_map<std::string, std::string> SDLGPUShader::PreProcess(const std::string& source)
    {
        std::unordered_map<std::string, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            LOG_CORE_ASSERT(eol != std::string::npos, "Shader syntax error");

            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSources[type] = source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    uint32_t SDLGPUShader::CountSamplers(const std::string& source)
    {
        uint32_t count = 0;

        std::regex samplerArrayRegex(R"(sampler2D\s+\w+\s*\[\s*(\d+)\s*\])");
        std::smatch match;
        if (std::regex_search(source, match, samplerArrayRegex))
        {
            return (uint32_t)std::stoi(match[1].str());
        }

        std::regex samplerRegex(R"(sampler2D\s+\w+)");
        for (auto it = std::sregex_iterator(source.begin(), source.end(), samplerRegex); it != std::sregex_iterator(); ++it)
        {
            ++count;
        }

        return count;
    }

    uint32_t SDLGPUShader::CountUniformBlocks(const std::string& source)
    {
        uint32_t count = 0;
        std::regex blockRegex(R"(layout\s*\(\s*std140[^\)]*\)\s*uniform\s+\w+)");
        for (auto it = std::sregex_iterator(source.begin(), source.end(), blockRegex); it != std::sregex_iterator(); ++it)
        {
            ++count;
        }
        return count;
    }

    std::string SDLGPUShader::BuildVertexLayoutSignature(const SDLGPUVertexArray& vertexArray)
    {
        std::stringstream ss;
        for (const auto& vb : vertexArray.GetVertexBuffers())
        {
            auto sdlvb = std::dynamic_pointer_cast<SDLGPUVertexBuffer>(vb);
            if (!sdlvb)
            {
                continue;
            }

            ss << "vb(" << sdlvb->GetLayout().GetStride() << "):";
            for (const auto& element : sdlvb->GetLayout())
            {
                ss << static_cast<int>(element.type) << '@' << element.offset << ';';
            }
            ss << '|';
        }

        return ss.str();
    }

    std::string SDLGPUShader::MakePipelineKey(const SDLGPUVertexArray& vertexArray, const SDLGPUShaderPassSignature& signature)
    {
        std::stringstream ss;
        ss << BuildVertexLayoutSignature(vertexArray)
           << "prim=" << signature.PrimitiveType
           << "|targets=" << signature.NumColorTargets
           << "|depth=" << signature.HasDepth << ':' << signature.DepthFormat
           << "|samples=" << signature.Samples
           << "|blend=" << signature.AlphaBlend;

        for (uint32_t i = 0; i < signature.NumColorTargets; ++i)
        {
            ss << "|cf" << i << '=' << signature.ColorFormats[i];
        }

        return ss.str();
    }

    void SDLGPUShader::LoadFromCombinedShader(const std::filesystem::path& glslPath)
    {
        auto shaderPath = PathService::ResolveAsset(glslPath.string());
        LOG_CORE_ASSERT(std::filesystem::exists(shaderPath.string()), "Shader source file not found: " + shaderPath.string());

        std::ifstream in(shaderPath, std::ios::in | std::ios::binary);
        LOG_CORE_ASSERT(in, "Failed to open shader source: {0}", shaderPath.string());

        std::stringstream ss;
        ss << in.rdbuf();
        const auto split = PreProcess(ss.str());

        auto vsIt = split.find("vertex");
        auto fsIt = split.find("fragment");
        LOG_CORE_ASSERT(vsIt != split.end(), "Vertex stage missing in shader: " + shaderPath.string());
        LOG_CORE_ASSERT(fsIt != split.end(), "Fragment stage missing in shader: " + shaderPath.string());

        m_VertexStage.Source = vsIt->second;
        m_VertexStage.Present = true;
        m_VertexStage.SamplerCount = CountSamplers(m_VertexStage.Source);
        m_VertexStage.UniformBufferCount = CountUniformBlocks(m_VertexStage.Source);

        m_FragmentStage.Source = fsIt->second;
        m_FragmentStage.Present = true;
        m_FragmentStage.SamplerCount = CountSamplers(m_FragmentStage.Source);
        m_FragmentStage.UniformBufferCount = CountUniformBlocks(m_FragmentStage.Source);

        LoadCompiledStage(shaderPath, shaderPath.stem().string(), "vert", m_VertexStage);
        LoadCompiledStage(shaderPath, shaderPath.stem().string(), "frag", m_FragmentStage);

        m_VertexUniformBufferCount = m_VertexStage.UniformBufferCount;
        m_FragmentUniformBufferCount = m_FragmentStage.UniformBufferCount;
        m_FragmentSamplerCount = m_FragmentStage.SamplerCount;

        // Vertex
        SDL_GPUShaderCreateInfo shaderCreateInfo {};
        shaderCreateInfo.code_size = m_VertexStage.Code.size();
        shaderCreateInfo.code = m_VertexStage.Code.data();
        shaderCreateInfo.entrypoint = m_VertexStage.EntryPoint.c_str();
        shaderCreateInfo.format = (SDL_GPUShaderFormat)m_VertexStage.Format;
        shaderCreateInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
        shaderCreateInfo.num_samplers = m_VertexStage.SamplerCount;
        shaderCreateInfo.num_storage_textures = 0;
        shaderCreateInfo.num_storage_buffers = 0;
        shaderCreateInfo.num_uniform_buffers = m_VertexStage.UniformBufferCount;

        m_VertexShader = SDL_CreateGPUShader( GetDevice(), &shaderCreateInfo );

        LOG_CORE_ASSERT(m_VertexShader, "SDL_CreateGPUShader(vertex) failed for {0}: {1}", m_Name, SDL_GetError());

        // Frament
        shaderCreateInfo.code_size = m_FragmentStage.Code.size();
        shaderCreateInfo.code = m_FragmentStage.Code.data();
        shaderCreateInfo.entrypoint = m_FragmentStage.EntryPoint.c_str();
        shaderCreateInfo.format = (SDL_GPUShaderFormat)m_FragmentStage.Format;
        shaderCreateInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
        shaderCreateInfo.num_samplers = m_FragmentStage.SamplerCount;
        shaderCreateInfo.num_storage_textures = 0;
        shaderCreateInfo.num_storage_buffers = 0;
        shaderCreateInfo.num_uniform_buffers = m_FragmentStage.UniformBufferCount;
        
        m_FragmentShader = SDL_CreateGPUShader( GetDevice(), &shaderCreateInfo );

        LOG_CORE_ASSERT(m_FragmentShader, "SDL_CreateGPUShader(fragment) failed for {0}: {1}", m_Name, SDL_GetError());
    }

    void SDLGPUShader::LoadCompiledStage(const std::filesystem::path& glslPath, const std::string& stem, const std::string& stageName, ShaderStageInfo& outInfo)
    {
        SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(GetDevice());

        std::filesystem::path compiledDir = glslPath.parent_path() / "Compiled";
        std::filesystem::path compiledPath;
        if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
        {
            compiledPath = compiledDir / "SPIRV" / (stem + "." + stageName + ".spv");
            outInfo.EntryPoint = "main";
            outInfo.Format = SDL_GPU_SHADERFORMAT_SPIRV;
        }
        else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
        {
            compiledPath = compiledDir / "MSL" / (stem + "." + stageName + ".msl");
            outInfo.EntryPoint = "main0";
            outInfo.Format = SDL_GPU_SHADERFORMAT_MSL;
        }
        else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
        {
            compiledPath = compiledDir / "DXIL" / (stem + "." + stageName + ".dxil");
            outInfo.EntryPoint = "main";
            outInfo.Format = SDL_GPU_SHADERFORMAT_DXIL;
        }
        else
        {
            LOG_CORE_ASSERT(false, "No supported SDL_GPU shader format available for shader {0}", m_Name);
        }

        LOG_CORE_ASSERT(std::filesystem::exists(compiledPath),
                        "Compiled shader stage not found. Expected: {0}. Compile your GLSL/HLSL into backend binaries first.",
                        compiledPath.string());

        std::ifstream file(compiledPath, std::ios::binary | std::ios::ate);
        LOG_CORE_ASSERT(file, "Failed to open compiled shader stage: {0}", compiledPath.string());
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        outInfo.Code.resize((size_t)size);
        if (size > 0)
        {
            file.read(reinterpret_cast<char*>(outInfo.Code.data()), size);
        }
    }
}
