#include <pch.h>
#include "Core/EngineApp.h"
#include "Render/Shader.h"
#include "Render/VertexArray.h"
#include "Render/Buffer.h"
#include "Render/GraphicsDevice.h"
#include "Core/PathService.h"

#include <SDL3/SDL_gpu.h>
#include <SDL3_shadercross/SDL_shadercross.h>

#include <fstream>
#include <regex>
#include <sstream>

namespace Cober {

const Shader* Shader::s_BoundShader = nullptr;

namespace {

static SDL_GPUDevice* GetDevice()
{
    return GraphicsDevice::Get().GetDevice();
}

// Maps ShaderDataType to SDL3 GPU vertex element format
SDL_GPUVertexElementFormat ToVertexFormat(ShaderDataType type)
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
    LOG_CORE_ASSERT(false, "Unsupported vertex attribute type");
    return SDL_GPU_VERTEXELEMENTFORMAT_INVALID;
}

} // anonymous namespace

// ======================================================================================
// Shader Construction / Destruction
// ======================================================================================

Shader::Shader(const char* fileName)
{
    ReadAndLoadShader(fileName);
}

Shader::Shader(const std::filesystem::path& filePath)
{
    (void)filePath;
    // TODO: Path-based loading
}

Shader::~Shader()
{
    SDL_GPUDevice* device = GraphicsDevice::Get().GetDevice();
    if (!device)
        return;

    for (auto& [_, pipeline] : m_PipelineCache)
        SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
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

// ======================================================================================
// Shader Loading
// ======================================================================================

void Shader::Bind() const
{
    s_BoundShader = this;
}

const Shader* Shader::GetBoundShader()
{
    return s_BoundShader;
}

void Shader::ReadAndLoadShader(const char* fileName)
{
    m_Name = fileName;

    // Source paths
    m_vertexShaderPath = PathService::ResolveAsset("shaders\\source\\"
        + std::string(fileName) + ShaderStageToStr(VERTEX) + ".hlsl");
    m_fragmentShaderPath = PathService::ResolveAsset("shaders\\source\\"
        + std::string(fileName) + ShaderStageToStr(FRAGMENT) + ".hlsl");

    m_VertexStage.Source = ReadSourceShader(m_vertexShaderPath);
    m_VertexStage.SamplerCount = CountSamplers(m_VertexStage.Source);
    m_VertexStage.UniformBufferCount = CountUniformBlocks(m_VertexStage.Source);
    m_VertexUniformBufferCount = m_VertexStage.UniformBufferCount;

    m_FragmentStage.Source = ReadSourceShader(m_fragmentShaderPath);
    m_FragmentStage.SamplerCount = CountSamplers(m_FragmentStage.Source);
    m_FragmentStage.UniformBufferCount = CountUniformBlocks(m_FragmentStage.Source);
    m_FragmentUniformBufferCount = m_FragmentStage.UniformBufferCount;
    m_FragmentSamplerCount = m_FragmentStage.SamplerCount;

    LOG_CORE_INFO("Shader '{0}': fragmentSamplerCount={1}, vertexUniformCount={2}, fragUniformCount={3}",
        fileName, m_FragmentSamplerCount, m_VertexUniformBufferCount, m_FragmentUniformBufferCount);

    // Load compiled bytecode and create SDL_GPUShader objects
    m_VertexShader   = LoadShader(PathService::ResolveAsset("shaders\\compiled"), VERTEX, m_VertexStage);
    m_FragmentShader = LoadShader(PathService::ResolveAsset("shaders\\compiled"), FRAGMENT, m_FragmentStage);
}

// ======================================================================================
// Pipeline Creation
// ======================================================================================

SDL_GPUGraphicsPipeline* Shader::GetOrCreatePipeline(
    const VertexArray& vertexArray,
    const ShaderPassSignature& signature)
{
    // Build a key from vertex layout + render target info for caching
    const std::string key = MakePipelineKey(vertexArray, signature);
    auto it = m_PipelineCache.find(key);
    if (it != m_PipelineCache.end())
        return it->second;

    // No cached pipeline — create one
    //
    // SDL_GPUGraphicsPipeline bundles:
    //   - Vertex and fragment shaders
    //   - Vertex input state (attribute descriptions matching the shader)
    //   - Primitive type (triangles, lines, etc.)
    //   - Rasterizer state (cull mode, fill mode)
    //   - Blend state per color target
    //   - Depth/stencil state
    //   - Render target formats (must match the active render pass)
    //
    if (!m_VertexShader || !m_FragmentShader) {
        LOG_CORE_ERROR("Cannot create pipeline for '{0}': shader stage(s) missing", m_Name);
        return nullptr;
    }

    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.vertex_shader = m_VertexShader;
    pipelineCreateInfo.fragment_shader = m_FragmentShader;
    pipelineCreateInfo.primitive_type = (SDL_GPUPrimitiveType)signature.PrimitiveType;

    // Build vertex input state from the VertexArray's buffer layouts
    const auto& vertexBuffers = vertexArray.GetVertexBuffers();
    std::vector<SDL_GPUVertexBufferDescription> bufferDescs;
    std::vector<SDL_GPUVertexAttribute> attributes;
    uint32_t attribLocation = 0;

    for (uint32_t slot = 0; slot < vertexBuffers.size(); ++slot)
    {
        const auto& vb = vertexBuffers[slot];
        const auto& layout = vb->GetLayout();

        SDL_GPUVertexBufferDescription desc{};
        desc.slot = slot;
        desc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
        desc.instance_step_rate = 0;
        desc.pitch = layout.GetStride();
        bufferDescs.push_back(desc);

        uint32_t offset = 0;
        for (const auto& element : layout)
        {
            SDL_GPUVertexAttribute attr{};
            attr.buffer_slot = slot;
            attr.format = ToVertexFormat(element.type);
            attr.location = attribLocation++;
            attr.offset = (uint32_t)element.offset;
            attributes.push_back(attr);
        }
    }

    SDL_GPUVertexInputState vertexInputState{};
    vertexInputState.num_vertex_buffers = (uint32_t)bufferDescs.size();
    vertexInputState.vertex_buffer_descriptions = bufferDescs.data();
    vertexInputState.num_vertex_attributes = (uint32_t)attributes.size();
    vertexInputState.vertex_attributes = attributes.data();
    pipelineCreateInfo.vertex_input_state = vertexInputState;

    // Target info: color formats + depth format
    std::vector<SDL_GPUColorTargetDescription> colorTargetDescs(signature.NumColorTargets);
    for (uint32_t i = 0; i < signature.NumColorTargets; ++i)
    {
        SDL_GPUColorTargetDescription& desc = colorTargetDescs[i];
        desc.format = (SDL_GPUTextureFormat)signature.ColorFormats[i];

        // Integer formats (R32_SINT, R32_UINT, etc.) do not support blending
        SDL_GPUTextureFormat fmt = desc.format;
        bool isIntegerFormat =
            (fmt >= SDL_GPU_TEXTUREFORMAT_R8_UINT  && fmt <= SDL_GPU_TEXTUREFORMAT_R32G32B32A32_UINT) ||
            (fmt >= SDL_GPU_TEXTUREFORMAT_R8_INT   && fmt <= SDL_GPU_TEXTUREFORMAT_R32G32B32A32_INT);

        if (signature.AlphaBlend && !isIntegerFormat)
        {
            desc.blend_state = SDL_GPUColorTargetBlendState{
                .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                .color_blend_op = SDL_GPU_BLENDOP_ADD,
                .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
                .color_write_mask = SDL_GPU_COLORCOMPONENT_R
                    | SDL_GPU_COLORCOMPONENT_G
                    | SDL_GPU_COLORCOMPONENT_B
                    | SDL_GPU_COLORCOMPONENT_A,
                .enable_blend = true,
                .enable_color_write_mask = false,
            };
        }
    }

    SDL_GPUGraphicsPipelineTargetInfo targetInfo{};
    targetInfo.num_color_targets = signature.NumColorTargets;
    targetInfo.color_target_descriptions = colorTargetDescs.data();
    if (signature.HasDepth)
    {
        targetInfo.has_depth_stencil_target = true;
        targetInfo.depth_stencil_format = (SDL_GPUTextureFormat)signature.DepthFormat;

        SDL_GPUDepthStencilState depthState{};
        depthState.enable_depth_test = true;
        depthState.enable_depth_write = true;
        depthState.compare_op = SDL_GPU_COMPAREOP_LESS;
        pipelineCreateInfo.depth_stencil_state = depthState;
    }
    pipelineCreateInfo.target_info = targetInfo;

    SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(GetDevice(), &pipelineCreateInfo);
    if (!pipeline) {
        LOG_CORE_ERROR("SDL_CreateGPUGraphicsPipeline failed for shader {0}: {1}", m_Name, SDL_GetError());
        return nullptr;
    }

    m_PipelineCache[key] = pipeline;
    return pipeline;
}

// ======================================================================================
// Shader Stage Loading
// ======================================================================================

void Shader::LoadCompiledStage(const std::filesystem::path& compiledDir, ShaderStage stageName, ShaderStageInfo& outInfo)
{
    SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(GetDevice());

    std::filesystem::path compiledPath;
    if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
        compiledPath = compiledDir / (m_Name + ShaderStageToStr(stageName) + ".spv");
        outInfo.EntryPoint = "main";
        outInfo.Format = SDL_GPU_SHADERFORMAT_SPIRV;
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
        compiledPath = compiledDir / (m_Name + ShaderStageToStr(stageName) + ".msl");
        outInfo.EntryPoint = "main0";
        outInfo.Format = SDL_GPU_SHADERFORMAT_MSL;
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL) {
        compiledPath = compiledDir / (m_Name + ShaderStageToStr(stageName) + ".dxil");
        outInfo.EntryPoint = "main";
        outInfo.Format = SDL_GPU_SHADERFORMAT_DXIL;
    }
    else {
        LOG_CORE_WARNING("Unrecognized backend shader format!");
        return;
    }

    outInfo.Code = ReadCompiledShader(compiledPath, outInfo.CodeSize);
    if (!outInfo.Code) {
        outInfo.Present = false;
        return;
    }
    outInfo.Present = true;
}

SDL_GPUShader* Shader::LoadShader(const std::filesystem::path& shaderPath, ShaderStage stageName, ShaderStageInfo& stageInfo)
{
    LoadCompiledStage(shaderPath, stageName, stageInfo);

    // Try runtime cross-compilation from HLSL source if compiled bytecode is missing
    if (!stageInfo.Code)
    {
        const std::string& hlslSource = (stageName == VERTEX) ? m_VertexStage.Source : m_FragmentStage.Source;

        if (hlslSource.empty()) {
            LOG_CORE_ERROR("Skipping shader creation for {0} {1}: no compiled bytecode and no HLSL source",
                m_Name, ShaderStageToStr(stageName));
            return nullptr;
        }

        LOG_CORE_INFO("Runtime cross-compiling {0} {1} from HLSL source...", m_Name, ShaderStageToStr(stageName));

        SDL_ShaderCross_ShaderStage scStage = (stageName == VERTEX)
            ? SDL_SHADERCROSS_SHADERSTAGE_VERTEX
            : SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT;

        // Step 1: Compile HLSL source to SPIR-V bytecode
        SDL_ShaderCross_HLSL_Info hlslInfo{};
        hlslInfo.source = hlslSource.c_str();
        hlslInfo.entrypoint = "main";
        hlslInfo.include_dir = nullptr;
        hlslInfo.defines = nullptr;
        hlslInfo.shader_stage = scStage;
        hlslInfo.props = 0;

        size_t spirvSize = 0;
        void* spirvCode = SDL_ShaderCross_CompileSPIRVFromHLSL(&hlslInfo, &spirvSize);
        if (!spirvCode) {
            LOG_CORE_ERROR("HLSL -> SPIRV cross-compilation failed for {0} {1}: {2}",
                m_Name, ShaderStageToStr(stageName), SDL_GetError());
            return nullptr;
        }

        // Step 2: Use SDL_shadercross to create the GPU shader directly (handles SPIR-V -> backend)
        SDL_ShaderCross_SPIRV_Info spirvInfo{};
        spirvInfo.bytecode = (const Uint8*)spirvCode;
        spirvInfo.bytecode_size = spirvSize;
        spirvInfo.entrypoint = "main";
        spirvInfo.shader_stage = scStage;
        spirvInfo.props = 0;

        SDL_ShaderCross_GraphicsShaderResourceInfo resourceInfo{};
        resourceInfo.num_samplers = stageInfo.SamplerCount;
        resourceInfo.num_storage_textures = 0;
        resourceInfo.num_storage_buffers = 0;
        resourceInfo.num_uniform_buffers = stageInfo.UniformBufferCount;

        auto shader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(
            GetDevice(), &spirvInfo, &resourceInfo, 0);

        SDL_free(spirvCode);

        if (!shader) {
            LOG_CORE_ERROR("SDL_ShaderCross_CompileGraphicsShaderFromSPIRV failed for {0} {1}: {2}",
                m_Name, ShaderStageToStr(stageName), SDL_GetError());
            return nullptr;
        }

        stageInfo.Present = true;
        LOG_CORE_INFO("Runtime cross-compilation succeeded for {0} {1}", m_Name, ShaderStageToStr(stageName));
        return shader;
    }
    else
    {
        SDL_GPUShaderCreateInfo createInfo{};
        createInfo.code = (const Uint8*)stageInfo.Code;
        createInfo.code_size = stageInfo.CodeSize;
        createInfo.entrypoint = stageInfo.EntryPoint.c_str();
        createInfo.format = (SDL_GPUShaderFormat)stageInfo.Format;
        createInfo.stage = (stageName == VERTEX)
            ? SDL_GPU_SHADERSTAGE_VERTEX
            : SDL_GPU_SHADERSTAGE_FRAGMENT;
        createInfo.num_samplers = stageInfo.SamplerCount;
        createInfo.num_storage_textures = 0;
        createInfo.num_storage_buffers = 0;
        createInfo.num_uniform_buffers = stageInfo.UniformBufferCount;
        createInfo.props = 0;

        auto shader = SDL_CreateGPUShader(GetDevice(), &createInfo);
        if (!shader)
        {
            LOG_CORE_ERROR("SDL_CreateGPUShader failed for {0} {1}: {2}",
                m_Name, ShaderStageToStr(stageName), SDL_GetError());
            return nullptr;
        }

        LOG_CORE_INFO("Loaded pre-compiled shader {0} {1}", m_Name, ShaderStageToStr(stageName));
        return shader;
    }
}

void* Shader::ReadCompiledShader(const std::filesystem::path& filePath, size_t& outCodeSize)
{
    void* code = SDL_LoadFile((const char*)filePath.string().c_str(), &outCodeSize);
    if (!code)
    {
        SDL_Log("Failed to load shader from disk! %s", filePath.string().c_str());
        return nullptr;
    }
    return code;
}

std::string Shader::ReadSourceShader(const std::filesystem::path& filePath)
{
    std::ifstream in(filePath, std::ios::in | std::ios::binary);
    if (!in) {
        LOG_CORE_ERROR("Failed to open shader source: {}", filePath.string());
        return {};
    }
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// ======================================================================================
// Pipeline Key Generation
// ======================================================================================

std::string Shader::BuildVertexLayoutSignature(const VertexArray& vertexArray)
{
    std::stringstream ss;
    for (const auto& vb : vertexArray.GetVertexBuffers())
    {
        ss << "vb(" << vb->GetLayout().GetStride() << "):";
        for (const auto& element : vb->GetLayout())
            ss << static_cast<int>(element.type) << '@' << element.offset << ';';
        ss << '|';
    }
    return ss.str();
}

std::string Shader::MakePipelineKey(const VertexArray& vertexArray, const ShaderPassSignature& signature)
{
    std::stringstream ss;
    ss << BuildVertexLayoutSignature(vertexArray)
       << "prim=" << signature.PrimitiveType
       << "|targets=" << signature.NumColorTargets
       << "|depth=" << signature.HasDepth << ':' << signature.DepthFormat
       << "|samples=" << signature.Samples
       << "|blend=" << signature.AlphaBlend;

    for (uint32_t i = 0; i < signature.NumColorTargets; ++i)
        ss << "|cf" << i << '=' << signature.ColorFormats[i];

    return ss.str();
}

// ======================================================================================
// Shader Source Introspection
// ======================================================================================

uint32_t Shader::CountSamplers(const std::string& source)
{
    // Try HLSL Texture2DArray pattern first: Texture2DArray<float4> name
    {
        uint32_t count = 0;
        std::regex hlslTex2DArrayRegex(R"(Texture2DArray\s*<[^>]+>\s+\w+)");
        for (auto it = std::sregex_iterator(source.begin(), source.end(), hlslTex2DArrayRegex); it != std::sregex_iterator(); ++it)
            ++count;
        if (count > 0)
            return count;
    }
    // Try HLSL Texture2D array pattern: Texture2D<float4> name[N]
    {
        std::regex hlslTexArrayRegex(R"(Texture2D\s*<[^>]+>\s+\w+\s*\[\s*(\d+)\s*\])");
        std::smatch match;
        if (std::regex_search(source, match, hlslTexArrayRegex))
            return (uint32_t)std::stoi(match[1].str());
    }
    // HLSL single Texture2D: Texture2D<float4> name
    {
        uint32_t count = 0;
        std::regex hlslTexRegex(R"(Texture2D\s*<[^>]+>\s+\w+)");
        for (auto it = std::sregex_iterator(source.begin(), source.end(), hlslTexRegex); it != std::sregex_iterator(); ++it)
            ++count;
        if (count > 0)
            return count;
    }
    // Fallback: GLSL sampler2D array pattern
    {
        std::regex samplerArrayRegex(R"(sampler2D\s+\w+\s*\[\s*(\d+)\s*\])");
        std::smatch match;
        if (std::regex_search(source, match, samplerArrayRegex))
            return (uint32_t)std::stoi(match[1].str());
    }
    // Fallback: GLSL single sampler2D
    {
        uint32_t count = 0;
        std::regex samplerRegex(R"(sampler2D\s+\w+)");
        for (auto it = std::sregex_iterator(source.begin(), source.end(), samplerRegex); it != std::sregex_iterator(); ++it)
            ++count;
        return count;
    }
}

uint32_t Shader::CountUniformBlocks(const std::string& source)
{
    // Try HLSL cbuffer pattern first: cbuffer Name : register(bN)
    {
        uint32_t count = 0;
        std::regex cbufferRegex(R"(cbuffer\s+\w+\s*:\s*register\s*\(\s*b\d+(\s*,\s*space\d+)?\s*\))");
        for (auto it = std::sregex_iterator(source.begin(), source.end(), cbufferRegex); it != std::sregex_iterator(); ++it)
            ++count;
        if (count > 0)
            return count;
    }
    // Fallback: GLSL layout(std140) uniform block pattern
    {
        uint32_t count = 0;
        std::regex blockRegex(R"(layout\s*\(\s*std140[^\)]*\)\s*uniform\s+\w+)");
        for (auto it = std::sregex_iterator(source.begin(), source.end(), blockRegex); it != std::sregex_iterator(); ++it)
            ++count;
        return count;
    }
}

const char* Shader::ShaderStageToStr(ShaderStage stage)
{
    switch (stage)
    {
        case VERTEX:   return ".vert";
        case FRAGMENT: return ".frag";
    }
    return nullptr;
}

// ======================================================================================
// ShaderLibrary
// ======================================================================================

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
{
    LOG_ASSERT(!Exists(name), "Shader already exists!");
    _shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
    auto& name = shader->GetName();
    Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
{
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
{
    auto shader = Shader::Create(filepath);
    Add(name, shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name)
{
    LOG_ASSERT(Exists(name), "Shader does not exist!");
    return _shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) const
{
    return _shaders.find(name) != _shaders.end();
}

Ref<Shader> Shader::Create(const char* fileName)
{
    return CreateRef<Shader>(fileName);
}

Ref<Shader> Shader::Create(const std::filesystem::path& filepath)
{
    return CreateRef<Shader>(filepath);
}

} // namespace Cober
