
#ifndef SDLGPU_SHADER_H
#define SDLGPU_SHADER_H

#include "Render/Shader.h"

struct SDL_GPUShader;
struct SDL_GPUGraphicsPipeline;

namespace Cober {

    class SDLGPUVertexArray;

    struct SDLGPUShaderPassSignature
    {
        uint32_t NumColorTargets = 0;
        std::array<int, 4> ColorFormats{ 0, 0, 0, 0 };
        int DepthFormat = 0;
        bool HasDepth = false;
        uint32_t Samples = 1;
        uint32_t PrimitiveType = 0;
        bool AlphaBlend = true;
    };

    class SDLGPUShader : public Shader
    {
    public:
        explicit SDLGPUShader(const std::string& filepath);
        SDLGPUShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        ~SDLGPUShader() override;

        void Bind() const override;
        void Unbind() const override {}

        const std::string& GetName() const override { return m_Name; }

        SDL_GPUGraphicsPipeline* GetOrCreatePipeline(const SDLGPUVertexArray& vertexArray, const SDLGPUShaderPassSignature& signature);

        uint32_t GetVertexUniformBufferCount() const { return m_VertexUniformBufferCount; }
        uint32_t GetFragmentUniformBufferCount() const { return m_FragmentUniformBufferCount; }
        uint32_t GetFragmentSamplerCount() const { return m_FragmentSamplerCount; }

        static const SDLGPUShader* GetBoundShader();

    private:
        struct ShaderStageInfo
        {
            std::string Source;
            std::vector<uint8_t> Code;
            std::string EntryPoint;
            int Format = 0;
            uint32_t SamplerCount = 0;
            uint32_t UniformBufferCount = 0;
            bool Present = false;
        };

        static std::unordered_map<std::string, std::string> PreProcess(const std::string& source);
        static uint32_t CountSamplers(const std::string& source);
        static uint32_t CountUniformBlocks(const std::string& source);
        static std::string BuildVertexLayoutSignature(const SDLGPUVertexArray& vertexArray);
        static std::string MakePipelineKey(const SDLGPUVertexArray& vertexArray, const SDLGPUShaderPassSignature& signature);

        void LoadFromCombinedShader(const std::filesystem::path& glslPath);
        void LoadCompiledStage(const std::filesystem::path& glslPath, const std::string& stem, const std::string& stageName, ShaderStageInfo& outInfo);

    private:
        std::string m_Name;
        ShaderStageInfo m_VertexStage;
        ShaderStageInfo m_FragmentStage;
        SDL_GPUShader* m_VertexShader = nullptr;
        SDL_GPUShader* m_FragmentShader = nullptr;
        uint32_t m_VertexUniformBufferCount = 0;
        uint32_t m_FragmentUniformBufferCount = 0;
        uint32_t m_FragmentSamplerCount = 0;
        std::unordered_map<std::string, SDL_GPUGraphicsPipeline*> m_PipelineCache;

        static const SDLGPUShader* s_BoundShader;
    };
}

#endif
