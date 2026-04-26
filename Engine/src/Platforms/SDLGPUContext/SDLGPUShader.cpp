
#include <pch.h>
#include "Core/EngineApp.h"
#include "Platforms/SDLGPU/SDLGPUShader.h"
#include "Platforms/SDLGPU/SDLGPUVertexArray.h"
#include "Platforms/SDLGPU/SDLGPUBuffer.h"
#include "Platforms/SDLGPU/SDLGPURenderAPI.h"
#include "Core/PathService.h"

#include <SDL3/SDL_gpu.h>
#include <SDL3_shadercross/SDL_shadercross.h>

#include <fstream>
#include <regex>

namespace Cober 
{

    // --------------------------------------------------------------------------------------

    const SDLGPUShader* SDLGPUShader::s_BoundShader = nullptr;

    // --------------------------------------------------------------------------------------
    
    static SDL_GPUDevice* GetDevice()
    {
        auto* api = SDLGPURenderAPI::Get();
        LOG_CORE_ASSERT(api, "SDLGPURenderAPI has not been initialized yet");
        return api->GetDevice();
    }

    // --------------------------------------------------------------------------------------

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

    // --------------------------------------------------------------------------------------

    SDLGPUShader::SDLGPUShader(const char* fileName)
    {
        ReadAndLoadShader(fileName);
    }

    // --------------------------------------------------------------------------------------

    SDLGPUShader::SDLGPUShader(const std::filesystem::path& filePath)
    {
        ReadAndLoadShader(filePath);
    }

    // --------------------------------------------------------------------------------------

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

    // --------------------------------------------------------------------------------------

    void SDLGPUShader::Bind() const
    {
        s_BoundShader = this;
    }
    
    // --------------------------------------------------------------------------------------

    const std::string& SDLGPUShader::ReadSourceShader(const std::filesystem::path& filePath)  
    {
        std::ifstream in(m_vertexShaderPath, std::ios::in | std::ios::binary);
        std::stringstream ss;
        LOG_CORE_ASSERT(in, "Failed to open shader source: {0}", m_vertexShaderPath.string());

        ss << in.rdbuf();

        return ss.str();
    }
    
    // --------------------------------------------------------------------------------------

    void* SDLGPUShader::ReadCompiledShader(const std::filesystem::path& filePath)  
    {
        size_t codeSize;
        void* code = SDL_LoadFile((const char*)filePath.string().c_str(), &codeSize);
        if (code == NULL)
        {
            SDL_Log("Failed to load shader from disk! %s", filePath.string());
            return NULL;
        }
     
        return code;
    }
    
    // --------------------------------------------------------------------------------------
    
    void SDLGPUShader::ReadAndLoadShader(const char* fileName) 
    {
        m_Name = fileName;
        // Source
        m_vertexShaderPath = PathService::ResolveAsset("shaders\\source\\" + std::string(fileName) + ShaderStageToStr(ShaderStage::VERTEX) + ".hlsl");
        LOG_CORE_ASSERT(std::filesystem::exists(m_vertexShaderPath.string()), "Shader source file not found: " + m_vertexShaderPath.string());
        
        m_fragmentShaderPath = PathService::ResolveAsset("shaders\\source\\" + std::string(fileName) + ShaderStageToStr(ShaderStage::FRAGMENT) + ".hlsl");
        LOG_CORE_ASSERT(std::filesystem::exists(m_fragmentShaderPath.string()), "Shader source file not found: " + m_fragmentShaderPath.string());       

        // Compiled
        const std::filesystem::path vertexCompiled = PathService::ResolveAsset("shaders\\compiled\\" + std::string(fileName) + ShaderStageToStr(ShaderStage::VERTEX) + ".spv");
        LOG_CORE_ASSERT(std::filesystem::exists(m_vertexShaderPath.string()), "Shader source file not found: " + m_vertexShaderPath.string());
        
        const std::filesystem::path fragmentCompiled  = PathService::ResolveAsset("shaders\\compiled\\" + std::string(fileName) + ShaderStageToStr(ShaderStage::FRAGMENT) + ".spv");
        LOG_CORE_ASSERT(std::filesystem::exists(m_fragmentShaderPath.string()), "Shader source file not found: " + m_fragmentShaderPath.string());       
        

        // Read Vertex shader          
        size_t codeSize;
        void* code = SDL_LoadFile((const char*)vertexCompiled.string().c_str(), &codeSize);
        if (code == NULL) {
            SDL_Log("Failed to load shader from disk! %s", vertexCompiled.string());
        }

        m_VertexStage.Source = ReadSourceShader(m_vertexShaderPath);
        m_VertexStage.Code = code;//ReadCompiledShader(vertexCompiled);
        m_VertexStage.CodeSize = codeSize;
        m_VertexStage.Present = true;
        m_VertexStage.SamplerCount = CountSamplers(m_VertexStage.Source);
        m_VertexStage.UniformBufferCount = CountUniformBlocks(m_VertexStage.Source);

        m_VertexUniformBufferCount = m_VertexStage.UniformBufferCount;

        // Read Fragment shader
        codeSize;
        code = SDL_LoadFile((const char*)fragmentCompiled.string().c_str(), &codeSize);
        if (code == NULL) {
            SDL_Log("Failed to load shader from disk! %s", fragmentCompiled.string());
        }

        m_FragmentStage.Source = ReadSourceShader(m_fragmentShaderPath);
        m_FragmentStage.Code = code;//ReadCompiledShader(fragmentCompiled);
        m_FragmentStage.CodeSize = codeSize;
        m_FragmentStage.Present = true;
        m_FragmentStage.SamplerCount = CountSamplers(m_FragmentStage.Source);
        m_FragmentStage.UniformBufferCount = CountUniformBlocks(m_FragmentStage.Source);

        m_FragmentUniformBufferCount = m_FragmentStage.UniformBufferCount;
        m_FragmentSamplerCount = m_FragmentStage.SamplerCount;
        
        m_VertexShader   = LoadShader(PathService::ResolveAsset("shaders\\compiled"), ShaderStage::VERTEX, m_VertexStage);
        m_FragmentShader = LoadShader(PathService::ResolveAsset("shaders\\compiled"), ShaderStage::FRAGMENT, m_FragmentStage);
    }
    
    // --------------------------------------------------------------------------------------
    
    const void SDLGPUShader::ReleaseShaders() 
    {    
        SDL_ReleaseGPUShader(GetDevice(), m_VertexShader);
        SDL_ReleaseGPUShader(GetDevice(), m_FragmentShader);
    }

    // --------------------------------------------------------------------------------------

    void SDLGPUShader::ReadAndLoadShader(const std::filesystem::path& filePath) 
    {
        #if 0
        m_Name = (const char*)filePath.stem().c_str();

        const char* shaderExtension = (const char*)filePath.stem().c_str();
        if (SDL_strstr(shaderExtension, ".vert"))
        {
            // Read Vertex shader          
            m_VertexStage.Source = ReadSourceShader(filePath);
            m_VertexStage.Code = ReadCompiledShader(filePath);
            m_VertexStage.CodeSize = size_t(m_VertexStage.Code);
            m_VertexStage.Present = true;
            m_VertexStage.SamplerCount = CountSamplers(m_VertexStage.Source);
            m_VertexStage.UniformBufferCount = CountUniformBlocks(m_VertexStage.Source);
    
            m_VertexUniformBufferCount = m_VertexStage.UniformBufferCount;
            m_VertexShader   = LoadShader(filePath.parent_path(), ShaderStage::VERTEX, m_VertexStage  );
        }
        else if (SDL_strstr(shaderExtension, ".frag"))
        {
            // Read Fragment shader
            m_FragmentStage.Source = ReadSourceShader(filePath);
            m_FragmentStage.Code = ReadCompiledShader(filePath);
            m_FragmentStage.CodeSize = size_t(m_FragmentStage.Code);
            m_FragmentStage.Present = true;
            m_FragmentStage.SamplerCount = CountSamplers(m_FragmentStage.Source);
            m_FragmentStage.UniformBufferCount = CountUniformBlocks(m_FragmentStage.Source);
    
            m_FragmentUniformBufferCount = m_FragmentStage.UniformBufferCount;
            m_FragmentSamplerCount = m_FragmentStage.SamplerCount;
            m_FragmentShader = LoadShader(filePath.parent_path(), ShaderStage::FRAGMENT, m_FragmentStage);
        }
        #endif
    }

    // --------------------------------------------------------------------------------------

    SDL_GPUShader* SDLGPUShader::LoadShader(const std::filesystem::path& shaderPath, const ShaderStage& stageName, ShaderStageInfo& stageInfo)
    {
        LoadCompiledStage(shaderPath, stageName, stageInfo);

        SDL_GPUShaderCreateInfo shaderCreateInfo {};
        shaderCreateInfo.code_size = stageInfo.CodeSize;
        shaderCreateInfo.code = (const Uint8*)stageInfo.Code;
        shaderCreateInfo.entrypoint = stageInfo.EntryPoint.c_str();
        shaderCreateInfo.format = (SDL_GPUShaderFormat)stageInfo.Format;
        // TODO: FIX THIS
        shaderCreateInfo.stage = stageName == ShaderStage::VERTEX ? SDL_GPU_SHADERSTAGE_VERTEX : SDL_GPU_SHADERSTAGE_FRAGMENT; 
        shaderCreateInfo.num_samplers = stageInfo.SamplerCount;
        shaderCreateInfo.num_storage_textures = 0;
        shaderCreateInfo.num_storage_buffers = 0;
        shaderCreateInfo.num_uniform_buffers = stageInfo.UniformBufferCount;
        
        auto shader = SDL_CreateGPUShader( GetDevice(), &shaderCreateInfo );
        if (shader == nullptr) {
            LOG_CORE_ERROR("Failed to create shader! {0} - {1}", shaderPath.string(), ShaderStageToStr(stageName));
		    SDL_free(stageInfo.Code);
            return NULL;
        }

        SDL_free(stageInfo.Code);

        LOG_CORE_ASSERT(shader, "SDL_CreateGPUShader(fragment) failed for {0}: {1}", m_Name, SDL_GetError());

        return shader;
    }

    // --------------------------------------------------------------------------------------

    void SDLGPUShader::LoadCompiledStage(const std::filesystem::path& compiledDir, const ShaderStage& stageName, ShaderStageInfo& outInfo)
    {
        SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(GetDevice());

        std::filesystem::path compiledPath;
        if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
            compiledPath = compiledDir / (m_Name + std::string(ShaderStageToStr(stageName)) + ".spv");
            outInfo.EntryPoint = "main";
            outInfo.Format = SDL_GPU_SHADERFORMAT_SPIRV;
        } else 
        if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
            compiledPath = compiledDir / (m_Name + std::string(ShaderStageToStr(stageName)) + ".msl");
            outInfo.EntryPoint = "main0";
            outInfo.Format = SDL_GPU_SHADERFORMAT_MSL;
        } else 
        if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL) {
            compiledPath = compiledDir / (m_Name + std::string(ShaderStageToStr(stageName)) + ".dxil");
            outInfo.EntryPoint = "main";
            outInfo.Format = SDL_GPU_SHADERFORMAT_DXIL;
        } else {
            LOG_CORE_WARNING("%s", "Unrecognized backend shader format!");
            return;
        }
    }

    // --------------------------------------------------------------------------------------

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

            SDL_GPUVertexBufferDescription buffDescription {};
            buffDescription.slot = bufferSlot;
            buffDescription.pitch = sdlvb->GetLayout().GetStride();
            buffDescription.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
            buffDescription.instance_step_rate = 0;
            vbDescriptions.push_back(buffDescription);

            for (const auto& element : sdlvb->GetLayout())
            {
                SDL_GPUVertexAttribute vertexAtt {};
                vertexAtt.location = location++;
                vertexAtt.buffer_slot = bufferSlot;
                vertexAtt.format = ToVertexFormat(element.type);
                vertexAtt.offset = (uint32_t)element.offset;
                attributes.push_back(vertexAtt);
            }

            ++bufferSlot;
        }

        std::array<SDL_GPUColorTargetDescription, 4> colorDescs{};
        for (uint32_t i = 0; i < signature.NumColorTargets; ++i)
        {
            //colorDescs[i].format = static_cast<SDL_GPUTextureFormat>(signature.ColorFormats[i]);
            colorDescs[i].format = SDL_GetGPUSwapchainTextureFormat(GetDevice(), EngineApp::GetWindow().GetRawWindow());
        }

        SDL_GPUGraphicsPipelineCreateInfo createInfo = {};
        createInfo.vertex_shader = m_VertexShader;
        createInfo.fragment_shader = m_FragmentShader;
        SDL_GPUVertexInputState vertexInputState {};
        vertexInputState.vertex_buffer_descriptions = vbDescriptions.data();
        vertexInputState.num_vertex_buffers = (uint32_t)vbDescriptions.size();
        vertexInputState.vertex_attributes = attributes.data();
        vertexInputState.num_vertex_attributes = (uint32_t)attributes.size();
        createInfo.vertex_input_state = vertexInputState;

        createInfo.primitive_type = static_cast<SDL_GPUPrimitiveType>(signature.PrimitiveType);

        SDL_GPUGraphicsPipelineTargetInfo pipelinteTargetInfo {};
        pipelinteTargetInfo.color_target_descriptions = colorDescs.data();
        pipelinteTargetInfo.num_color_targets = signature.NumColorTargets;
        pipelinteTargetInfo.depth_stencil_format = static_cast<SDL_GPUTextureFormat>(signature.DepthFormat);
        pipelinteTargetInfo.has_depth_stencil_target = signature.HasDepth;
        createInfo.target_info = pipelinteTargetInfo;

        SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(GetDevice(), &createInfo);
        LOG_CORE_ASSERT(pipeline, "SDL_CreateGPUGraphicsPipeline failed for shader {0}: {1}", m_Name, SDL_GetError());

        m_PipelineCache[key] = pipeline;
        return pipeline;
    }

    // --------------------------------------------------------------------------------------

    const SDLGPUShader* SDLGPUShader::GetBoundShader()
    {
        return s_BoundShader;
    }

    // --------------------------------------------------------------------------------------

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

    // --------------------------------------------------------------------------------------

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

    // --------------------------------------------------------------------------------------

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

    // --------------------------------------------------------------------------------------

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

    // --------------------------------------------------------------------------------------

    const char* SDLGPUShader::ShaderStageToStr(const ShaderStage& shaderStage) 
    {
        switch(shaderStage) {
            case ShaderStage::VERTEX:   return ".vert";
            case ShaderStage::FRAGMENT: return ".frag";
            default:
                LOG_CORE_WARNING("Shader stage dont detected, format is wrong");
                return NULL;
        }
    }

    // --------------------------------------------------------------------------------------
}