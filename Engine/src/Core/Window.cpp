#include <pch.h>
#include <Core/Window.h>
#include <Core/Core.h>
#include <Core/EngineApp.h>

#include "Render/GraphicsDevice.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"

#include <SDL3/SDL.h>

namespace Cober
{
    static uint8_t s_SDL3WindowCount = 0;

    // --------------------------------------------------------------------------------------

    Window::Window(const WindowProps& props)
    {
        Init(props);
        LOG_CORE_INFO("Window Created");
    }

    // --------------------------------------------------------------------------------------

    Window::~Window()
    {
        Shutdown();
    }

    // --------------------------------------------------------------------------------------

    void Window::Init(const WindowProps& props)
    {
        CB_PROFILE_FUNCTION();

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

        m_Window = SDL_CreateWindow(m_Data.Title.c_str(), (int)props.Width, (int)props.Height, SDL_WINDOW_RESIZABLE);
        if (m_Window == nullptr)
        {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
            return;
        }

        m_initialized = true;
        ++s_SDL3WindowCount;

        // Initialize the GraphicsDevice (creates SDL_GPUDevice, claims window, sets up swapchain)
        GraphicsDevice::Init(m_Window);

        SetVSync(props.VSync);
    }

    // --------------------------------------------------------------------------------------

    void Window::Shutdown()
    {
        GraphicsDevice::Shutdown();

        SDL_DestroyWindow(m_Window);
        LOG_CORE_INFO("Window destroyed");
        m_Window = nullptr;

        if (m_initialized && --s_SDL3WindowCount == 0) {
            SDL_Quit();
            LOG_CORE_INFO("SDL Quit");
        }
    }

    // --------------------------------------------------------------------------------------

    void Window::OnUpdate()
    {
        // Reserved for per-frame window operations
    }

    // --------------------------------------------------------------------------------------

    void Window::SetVSync(bool enabled)
    {
        // VSync is set during GraphicsDevice::Init via SDL_SetGPUSwapchainParameters.
        // Toggling it at runtime would require re-creating the swapchain.
        m_Data.VSync = enabled;
    }

    // --------------------------------------------------------------------------------------

    bool Window::IsVSync() const
    {
        return m_Data.VSync;
    }

    // --------------------------------------------------------------------------------------

    void Window::ChangeFullScreen()
    {
        SDL_SetWindowFullscreen(m_Window, m_Data.FullScreen);
        m_Data.FullScreen = !m_Data.FullScreen;
    }

    // --------------------------------------------------------------------------------------
}
