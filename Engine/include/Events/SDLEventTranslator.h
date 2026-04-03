#ifndef TRANSLATED_SDL_EVENTS_H
#define TRANSLATED_SDL_EVENTS_H

#include "Core/Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include <SDL3/SDL.h>

namespace Cober
{
    Unique<Event> TranslateSDLEvent(const SDL_Event& raw)
    {
        switch (raw.type)
        {
            case SDL_EVENT_QUIT:
                return CreateUnique<WindowCloseEvent>();

            case SDL_EVENT_WINDOW_RESIZED:
                return CreateUnique<WindowResizeEvent>(
                    static_cast<uint32_t>(raw.window.data1),
                    static_cast<uint32_t>(raw.window.data2)
                );

            case SDL_EVENT_WINDOW_FOCUS_GAINED:
                return CreateUnique<WindowFocusEvent>();

            case SDL_EVENT_WINDOW_FOCUS_LOST:
                return CreateUnique<WindowLostFocusEvent>();

            case SDL_EVENT_WINDOW_MOVED:
                return CreateUnique<WindowMovedEvent>(
                    raw.window.data1,
                    raw.window.data2
                );

            case SDL_EVENT_KEY_DOWN:
                if (raw.key.repeat)
                    return nullptr;

                return CreateUnique<KeyPressedEvent>(
                    static_cast<KeyCode>(raw.key.scancode),
                    raw.key.repeat
                );

            case SDL_EVENT_KEY_UP:
                return CreateUnique<KeyReleasedEvent>(
                    static_cast<KeyCode>(raw.key.scancode)
                );

            case SDL_EVENT_MOUSE_MOTION:
                return CreateUnique<MouseMovedEvent>(
                    raw.motion.x,
                    raw.motion.y
                );

            case SDL_EVENT_MOUSE_WHEEL:
                return CreateUnique<MouseScrolledEvent>(
                    raw.wheel.x,
                    raw.wheel.y
                );

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                return CreateUnique<MouseButtonPressedEvent>(
                    static_cast<MouseButton>(raw.button.button)
                );

            case SDL_EVENT_MOUSE_BUTTON_UP:
                return CreateUnique<MouseButtonReleasedEvent>(
                    static_cast<MouseButton>(raw.button.button)
                );

            default:
                return nullptr;
        }
    }
}

#endif