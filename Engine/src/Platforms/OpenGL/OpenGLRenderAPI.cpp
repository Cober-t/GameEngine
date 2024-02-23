#include "OpenGL/OpenGLRenderAPI.h"

namespace Cober {

	void OpenGLRenderAPI::Init() {

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_ONE, GL_ZERO);

		//glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {

		GLCallV(glViewport(0, 0, width, height));
	}

	void OpenGLRenderAPI::SetClearColor(glm::vec4 color) {

		GLCallV(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRenderAPI::SetClearColor(float red, float green, float blue, float black) {

		float r = red / 255, g = green / 255, b = blue / 255, k = black / 255;
		GLCallV(glClearColor(r, g, b, k));
	}

	void OpenGLRenderAPI::Clear() {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	/*void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		GLCallV(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRenderAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		GLCallV(glDrawArrays(GL_LINES, 0, vertexCount));
	}*/

	void OpenGLRenderAPI::SetLineWidth(float width)
	{
		GLCallV(glLineWidth(width));
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