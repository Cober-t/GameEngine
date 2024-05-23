#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include "Core/Core.h"
#include "Render/GraphicsContext.h"

#include "Events/Event.h"

namespace Cober {

	struct WindowProps 
	{
		std::string Title;
		uint32_t Width, Height;
		bool Decorated = true;
		bool Fullscreen = false;
		bool VSync = true;

		WindowProps(const std::string& title = "Game Engine ;)",
					uint32_t width = 1280, 
					uint32_t height = 720, 
					bool vsync = true) 
				: Title(title), Width(width), Height(height), VSync(vsync)
			{
			}
	};

	class CB_API Window 
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProps& props);
		virtual ~Window();

		void OnUpdate();

		std::string& GetTitle() { return m_Data.Title; }	
		uint32_t GetWidth() const { return m_Data.Width; }
		uint32_t GetHeight() const { return m_Data.Height; }
		bool GetVsync() const { return m_Data.VSync; }

		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled);
		bool IsVSync() const;
		void ChangeFullScreen();

		virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void SetWindowCallbacks();
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		Unique<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			bool FullScreen;
			bool Decorated;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

#endif