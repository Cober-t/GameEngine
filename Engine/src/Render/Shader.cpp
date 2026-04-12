
#include <pch.h>
#include "Render/RenderAPI.h"
#include "Render/Shader.h"

#include "Platforms/SDLGPU/SDLGPUShader.h"

namespace Cober {

    Ref<Shader> Shader::Create(const char* fileName)
    {
        switch (RenderAPI::GetAPI()) {
            case RenderAPI::API::None:          LOG_CORE_ERROR("RenderAPI::None means there is not render defined!!");       return nullptr;
            case RenderAPI::API::SDLGPU:        return CreateRef<SDLGPUShader>(fileName);
            default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
        }

        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::filesystem::path& filePath)
    {
        switch (RenderAPI::GetAPI()) {
            case RenderAPI::API::None:          LOG_CORE_ERROR("RenderAPI::None means there is not render defined!!");       return nullptr;
            case RenderAPI::API::SDLGPU:        return CreateRef<SDLGPUShader>(filePath);
            default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
        }

        return nullptr;
    }

    // Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    // {
    //     switch (RenderAPI::GetAPI())
    //     {
    //         case RenderAPI::API::None:          LOG_CORE_ERROR("RenderAPI::None means there is not render defined!!");       return nullptr;
    //         case RenderAPI::API::SDLGPU:        return CreateRef<SDLGPUShader>(name, vertexSrc, fragmentSrc);
    //         default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
    //     }

    //     return nullptr;
    // }

    void Shader::SetInt(const std::string& name, int value) { (void)name; (void)value; }
    void Shader::SetIntArray(const std::string& name, int* values, uint32_t count) { (void)name; (void)values; (void)count; }
    void Shader::SetFloat(const std::string& name, float value) { (void)name; (void)value; }
    void Shader::SetFloat3(const std::string& name, const glm::vec3& value) { (void)name; (void)value; }
    void Shader::SetFloat4(const std::string& name, const glm::vec4& value) { (void)name; (void)value; }
    void Shader::SetVec2(const std::string& name, const glm::vec2& value) { (void)name; (void)value; }
    void Shader::SetVec3(const std::string& name, const glm::vec3& value) { (void)name; (void)value; }
    void Shader::SetVec4(const std::string& name, const glm::vec4& value) { (void)name; (void)value; }
    void Shader::SetMat3(const std::string& name, const glm::mat3& value) { (void)name; (void)value; }
    void Shader::SetMat4(const std::string& name, const glm::mat4& value) { (void)name; (void)value; }

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
}
