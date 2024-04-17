#include <pch.h>
#include "Platforms/OpenGL/OpenGLContext.h"

namespace Cober {

	
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		LOG_CORE_ASSERT(windowHandle, "Window handle is null!");
		LOG_CORE_INFO("Window Context Created!!");
	}

	OpenGLContext::~OpenGLContext()
	{
		LOG_CORE_INFO("Window Context Destroyed!!");
	}


	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		LOG_CORE_ASSERT(status, "Failed to initialize Glad!");

		LOG_CORE_INFO("OpenGL Info:");
		LOG_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		LOG_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		LOG_CORE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));

		LOG_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), \
						"The Engine requires at least OpenGL version 4.5!");
	}


	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}