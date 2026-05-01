
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
        explicit SDLGPUShader(const char* fileName);
        explicit SDLGPUShader(const std::filesystem::path& filePath);
        ~SDLGPUShader() override;

        void Bind() const override;
        void Unbind() const override {}

        const std::string& GetName() const override { return m_Name; }

        void ReadAndLoadShader(const char* shaderFilename);
        void ReadAndLoadShader(const std::filesystem::path& shaderFilePath);
        
        SDL_GPUGraphicsPipeline* GetOrCreatePipeline(
                            //const SDLGPUVertexArray& vertexArray, 
                            //const SDLGPUShaderPassSignature& signature
                        );

        uint32_t GetVertexUniformBufferCount() const { return m_VertexUniformBufferCount; }
        uint32_t GetFragmentUniformBufferCount() const { return m_FragmentUniformBufferCount; }
        uint32_t GetFragmentSamplerCount() const { return m_FragmentSamplerCount; }

        static const SDLGPUShader* GetBoundShader();
        // Only to thes shader
        inline SDL_GPUShader* GetVertexShader() override { return m_VertexShader; }
        inline SDL_GPUShader* GetFragmentShader() override { return m_FragmentShader; }
        const void ReleaseShaders() override;

    private:
        enum ShaderStage
        {
            VERTEX, FRAGMENT
        };
        struct ShaderStageInfo
        {
            void* Code;
            size_t CodeSize;
            std::string Source;
            std::string EntryPoint;
            int Format = 0;
            uint32_t SamplerCount = 0;
            uint32_t UniformBufferCount = 0;
            bool Present = false;
        };

        static uint32_t CountSamplers(const std::string& source);
        static uint32_t CountUniformBlocks(const std::string& source);
        static std::string BuildVertexLayoutSignature(const SDLGPUVertexArray& vertexArray);
        static std::string MakePipelineKey(const SDLGPUVertexArray& vertexArray, const SDLGPUShaderPassSignature& signature);

        const std::string& ReadSourceShader(const std::filesystem::path& shaderFilePath);
        void* ReadCompiledShader(const std::filesystem::path& shaderFilePath);
        SDL_GPUShader* LoadShader(const std::filesystem::path& shaderPath, const ShaderStage& stageName, ShaderStageInfo& stageInfo);

        void LoadCompiledStage(const std::filesystem::path& compiledDir, const ShaderStage& stageName, ShaderStageInfo& outInfo);
        const char* ShaderStageToStr(const ShaderStage& shaderStage);

    private:
        const char* m_Name;
        ShaderStageInfo m_VertexStage;
        ShaderStageInfo m_FragmentStage;
        SDL_GPUShader* m_VertexShader = nullptr;
        SDL_GPUShader* m_FragmentShader = nullptr;
        uint32_t m_VertexUniformBufferCount = 0;
        uint32_t m_FragmentUniformBufferCount = 0;
        uint32_t m_FragmentSamplerCount = 0;

        std::filesystem::path m_vertexShaderPath;
        std::filesystem::path m_fragmentShaderPath;
        std::unordered_map<std::string, SDL_GPUGraphicsPipeline*> m_PipelineCache;

        static const SDLGPUShader* s_BoundShader;
    };
}

#endif
