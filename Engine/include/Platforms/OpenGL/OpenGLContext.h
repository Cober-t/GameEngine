#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "Render/GraphicsContext.h"

struct GLFWwindow;

namespace Cober {

	class OpenGLContext : public GraphicsContext 
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual ~OpenGLContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}

#endif
