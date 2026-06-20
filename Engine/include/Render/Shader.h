#ifndef SHADER_H
#define SHADER_H

#include "Core/Core.h"

// --------------------------------------------------------------------------------------
// SDL3 GPU Shader System
//
// Shaders in SDL3 GPU are pre-compiled SPIR-V, DXIL, or MSL bytecode files.
// The backend (Vulkan/D3D12/Metal) determines which format is used.
// SDL_shadercross can be used for runtime cross-compilation.
//
// Pipeline objects bundle shaders with fixed-function state:
//   - Vertex layout (vertex attribute descriptions)
//   - Primitive type (triangles, lines, etc.)
//   - Rasterizer state (cull mode, fill mode, etc.)
//   - Render target formats (color attachment formats, depth format)
//
// The GPU API requires pipelines to match the render pass's target formats.
// This is why we cache pipelines keyed by vertex layout + target info.
// --------------------------------------------------------------------------------------

#include <array>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct SDL_GPUDevice;
struct SDL_GPUShader;
struct SDL_GPUGraphicsPipeline;

namespace Cober {

class VertexArray;

// Forward declare types used in pipeline creation
struct ShaderPassSignature
{
    uint32_t NumColorTargets = 0;
    std::array<int, 4> ColorFormats{ 0, 0, 0, 0 };
    int DepthFormat = 0;
    bool HasDepth = false;
    uint32_t Samples = 1;
    uint32_t PrimitiveType = 0;
    bool AlphaBlend = true;
};

// Shader wraps a pair of vertex+fragment SDL_GPUShader objects and manages
// a cache of SDL_GPUGraphicsPipeline variants (one per vertex layout + target format combo).
class CB_API Shader
{
public:
    explicit Shader(const char* fileName);
    explicit Shader(const std::filesystem::path& filePath);
    ~Shader();

    // Sets this shader as the active shader for subsequent draw calls
    void Bind() const;
    void Unbind() const {}

    void SetInt(const std::string& name, int value) { (void)name; (void)value; } // TODO: implement uniform setting

    const std::string& GetName() const { return m_Name; }

    // Load shader source and compile/load bytecode from disk
    void ReadAndLoadShader(const char* shaderFilename);

    // Returns or creates a pipeline matching the given vertex array layout + target format.
    // Pipelines are cached so creating one with the same parameters is cheap.
    SDL_GPUGraphicsPipeline* GetOrCreatePipeline(
        const VertexArray& vertexArray,
        const ShaderPassSignature& signature
    );

    // Introspection helpers (parsed from HLSL source)
    uint32_t GetVertexUniformBufferCount() const { return m_VertexUniformBufferCount; }
    uint32_t GetFragmentUniformBufferCount() const { return m_FragmentUniformBufferCount; }
    uint32_t GetFragmentSamplerCount() const { return m_FragmentSamplerCount; }

    // Raw SDL3 GPU shader handles (used during pipeline creation)
    SDL_GPUShader* GetVertexShader() const { return m_VertexShader; }
    SDL_GPUShader* GetFragmentShader() const { return m_FragmentShader; }

    static const Shader* GetBoundShader();
    static Ref<Shader> Create(const char* fileName);
    static Ref<Shader> Create(const std::filesystem::path& filepath);

private:
    enum ShaderStage { VERTEX, FRAGMENT };

    struct ShaderStageInfo
    {
        void* Code = nullptr;
        size_t CodeSize = 0;
        std::string Source;
        std::string EntryPoint = "main";
        int Format = 0;
        uint32_t SamplerCount = 0;
        uint32_t UniformBufferCount = 0;
        bool Present = false;
    };

    void LoadCompiledStage(const std::filesystem::path& compiledDir, ShaderStage stageName, ShaderStageInfo& outInfo);
    SDL_GPUShader* LoadShader(const std::filesystem::path& shaderPath, ShaderStage stageName, ShaderStageInfo& stageInfo);
    void* ReadCompiledShader(const std::filesystem::path& filePath, size_t& outCodeSize);
    std::string ReadSourceShader(const std::filesystem::path& filePath);

    static uint32_t CountSamplers(const std::string& source);
    static uint32_t CountUniformBlocks(const std::string& source);
    static std::string BuildVertexLayoutSignature(const VertexArray& vertexArray);
    static std::string MakePipelineKey(const VertexArray& vertexArray, const ShaderPassSignature& signature);
    static const char* ShaderStageToStr(ShaderStage stage);

    std::string m_Name;
    ShaderStageInfo m_VertexStage;
    ShaderStageInfo m_FragmentStage;
    SDL_GPUShader* m_VertexShader = nullptr;
    SDL_GPUShader* m_FragmentShader = nullptr;

    uint32_t m_VertexUniformBufferCount = 0;
    uint32_t m_FragmentUniformBufferCount = 0;
    uint32_t m_FragmentSamplerCount = 0;

    std::filesystem::path m_vertexShaderPath;
    std::filesystem::path m_fragmentShaderPath;

    // Pipeline cache: maps vertex layout + target signature -> pipeline
    std::unordered_map<std::string, SDL_GPUGraphicsPipeline*> m_PipelineCache;

    static const Shader* s_BoundShader;
};

// ShaderLibrary is a simple registry for named shaders
class CB_API ShaderLibrary
{
public:
    void Add(const std::string& name, const Ref<Shader>& shader);
    void Add(const Ref<Shader>& shader);
    Ref<Shader> Load(const std::string& filepath);
    Ref<Shader> Load(const std::string& name, const std::string& filepath);
    Ref<Shader> Get(const std::string& name);
    bool Exists(const std::string& name) const;

private:
    std::unordered_map<std::string, Ref<Shader>> _shaders;
};

}

#endif
