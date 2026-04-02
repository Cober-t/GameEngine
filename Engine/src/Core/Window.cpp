#include <pch.h>
#include <Core/Window.h>
#include <Core/Core.h>
#include <Core/EngineApp.h>

#include "Events/ApplicationEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"


namespace Cober {

	static uint8_t s_SDL3WindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}


	Window::Window(const WindowProps& props)
	{
		Init(props);
		LOG_CORE_INFO("Window Created");
	}

	Window::~Window() 
	{
		Shutdown();
	}

	void Window::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.VSync = props.VSync;
		m_Data.FullScreen = props.Fullscreen;
		m_Data.Decorated = props.Decorated;

		LOG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_SDL3WindowCount == 0)
		{
			if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
			{
				LOG_CORE_ERROR("Error: SDL_Init failed: {}", SDL_GetError());
				return;
			}
		}

		// #if defined(CB_DEBUG)
			// if (RenderAPI::GetAPI() == RenderAPI::API::OpenGL)
			// 	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		// #endif

		m_Window = SDL_CreateWindow( m_Data.Title.c_str(), (int)props.Width, (int)props.Height, SDL_WINDOW_RESIZABLE );
		if (m_Window == nullptr) 
		{
			SDL_Log( "Window could not be created! SDL error: %s\n", SDL_GetError() );
			return;
		}
        
		++s_SDL3WindowCount;

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		// glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(props.VSync);

		SetWindowCallbacks();
	}


	void Window::Shutdown()
	{
		SDL_ReleaseWindowFromGPUDevice(m_Context->GetGPUDevice(), m_Window);
		LOG_CORE_INFO("Removed GPU Device from the Window");
		SDL_DestroyWindow(m_Window);
		//LOG_CORE_INFO("Window Destroyed");
		m_Context->Destroy();
		//LOG_CORE_INFO("Window Context Destroyed!!");
		
		if(--s_SDL3WindowCount == 0) {
			SDL_Quit();
			LOG_CORE_INFO("SDL Quit");
		}
	}

	// void Window::SetEventCallback(const EventCallbackFn& callback)
	// { 
	// 	m_Data.EventCallback = callback; 
	// }

	// void* Window::GetNativeWindow() const
	// {
	// 	return m_Window;
	// }

	// uint32_t Window::GetWidth() const { return m_Data.Width; }
	// uint32_t Window::GetHeight() const { return m_Data.Height; }

	void Window::SetWindowCallbacks()
	{
		/// TODO: Process Events from SDL  (EventHandler)
		// EventHandler::Get()->ProcessEvents(event);
		// glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		// {
		// 	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		// 	data.Width = width;
		// 	data.Height = height;

		// 	WindowResizeEvent event(width, height);
		// 	data.EventCallback(event);
		// });
	}

	// 	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
	// 	{
	// 		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	// 		WindowCloseEvent event;
	// 		data.EventCallback(event);
	// 	});

	// 	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	// 	{
	// 		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
	// 		switch (action)
	// 		{
	// 			case GLFW_PRESS:
	// 			{
	// 				Input::UpdateKeyState((KeyCode)key, KeyState::Pressed);
	// 				KeyPressedEvent event((KeyCode)key, 0);
	// 				data.EventCallback(event);
	// 				break;
	// 			}
	// 			case GLFW_RELEASE:
	// 			{
	// 				Input::UpdateKeyState((KeyCode)key, KeyState::Released);
	// 				KeyReleasedEvent event((KeyCode)key);
	// 				data.EventCallback(event);
	// 				break;
	// 			}
	// 			case GLFW_REPEAT:
	// 			{
	// 				Input::UpdateKeyState((KeyCode)key, KeyState::Held);
	// 				KeyPressedEvent event((KeyCode)key, true);
	// 				data.EventCallback(event);
	// 				break;
	// 			}
	// 		}
	// 	});

	// 	glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
	// 	{
	// 		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	// 		KeyTypedEvent event((KeyCode)keycode);
	// 		data.EventCallback(event);
	// 	});

	// 	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
	// 	{
	// 		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	// 		switch (action)
	// 		{
	// 			case GLFW_PRESS:
	// 			{
	// 				Input::UpdateButtonState((MouseButton)button, KeyState::Pressed);
	// 				MouseButtonPressedEvent event((MouseButton)button);
	// 				data.EventCallback(event);
	// 				break;
	// 			}
	// 			case GLFW_RELEASE:
	// 			{
	// 				Input::UpdateButtonState((MouseButton)button, KeyState::Released);
	// 				MouseButtonReleasedEvent event((MouseButton)button);
	// 				data.EventCallback(event);
	// 				break;
	// 			}
	// 		}
	// 	});

	// 	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
	// 	{
	// 		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	// 		MouseScrolledEvent event((float)xOffset, (float)yOffset);
	// 		data.EventCallback(event);
	// 	});

	/// TODO: Process Events from SDL  (EventHandler)
	// EventHandler::Get()->ProcessEvents(event);
	// 	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
	// 	{
	// 		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	// 		MouseMovedEvent event((float)xPos, (float)yPos);
	// 		data.EventCallback(event);
	// 	});
	// }


	void Window::OnUpdate()
	{
		/// TODO: MANAGE SWAPBUFFER WINDOWS WITH SDL
		// m_Context->SwapBuffers();

		// TEST
		RenderGlobals::Clear();
	}


	void Window::SetVSync(bool enabled)
	{
		/// TODO: MANAGE VSYNC WITH SDL
		// if (enabled)
			// glfwSwapInterval(1);
		// else
			// glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}


	bool Window::IsVSync() const
	{
		return m_Data.VSync;
	}


	void Window::ChangeFullScreen() 
	{
		/// TODO: MANAGE WINDOWS PROPERTIES WITH SDL3
		SDL_SetWindowFullscreen(m_Window, m_Data.FullScreen);
		m_Data.FullScreen = !m_Data.FullScreen;
		// if (m_Data.FullScreen)
		// {
		// 	glfwRestoreWindow(m_Window);
		// }
		// else
		// {
		// 	glfwMaximizeWindow(m_Window);
		// }
	}
}