#include "Render/RenderAPI.h"
#include "Shader.h"

#include "Platforms/OpenGL/OpenGLShader.h"


namespace Cober {

	Ref<Shader> Shader::Create(const std::string& filepath) 
	{
		switch (RenderAPI::GetAPI()) {
			case RenderAPI::API::None:			LOG_CORE_ERROR("RenderAPI::None means there is not render defined!!");		return nullptr;
			case RenderAPI::API::OpenGL:		return CreateRef<OpenGLShader>(filepath);
			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}

		return nullptr;
	}


	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None:			LOG_CORE_ERROR("RenderAPI::None means there is not render defined!!");		return nullptr;
			case RenderAPI::API::OpenGL:		return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
			default:	LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}

		return nullptr;
	}


	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		LOG_ASSERT(Exists(name), "Shader already exists!");
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
		LOG_ASSERT(!Exists(name), "Shader already exists!");
		return _shaders[name];
	}


	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return _shaders.find(name) != _shaders.end();
	}
}