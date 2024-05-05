#include <pch.h>
#include "Platforms/OpenGL/OpenGLRenderAPI.h"

namespace Cober {

	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         LOG_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       LOG_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          LOG_CORE_WARNING(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_CORE_TRACE(message); return;
		}
		
		LOG_CORE_ASSERT(false, "Unknown severity level!");
	}


	void OpenGLRenderAPI::Init() 
	{

	// #ifdef CB_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	// #endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_LINE_SMOOTH);

		LOG_CORE_TRACE("Render API init");
	}


	void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) 
	{
		glViewport(0, 0, width, height);
	}


	void OpenGLRenderAPI::SetClearColor(glm::vec4 color) 
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}


	void OpenGLRenderAPI::SetClearColor(float red, float green, float blue, float black) 
	{
		float r = red / 255, g = green / 255, b = blue / 255, k = black / 255;
		glClearColor(r, g, b, k);
	}


	void OpenGLRenderAPI::Clear() 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	

	void OpenGLRenderAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRenderAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}


	void OpenGLRenderAPI::ClearErrors() 
	{
		while (glGetError());
	}


	void OpenGLRenderAPI::CheckErrors(const char* function) 
	{
		while (GLenum error = glGetError()) {
			std::string fileName = (std::string)__FILE__;
			//std::string solutionDir = SOLUTION_DIR;
			//fileName = fileName.substr(solutionDir.length());

			fileName = fileName.substr(fileName.find_last_of("\\") + 1);
			std::string errMessage = (const char*)glGetString(error);
			printf("%s", errMessage.c_str());
			LOG_CORE_ASSERT(false, "[OpenGL Error] (" + errMessage + ") " + function + " " + fileName);
		}
	}
}