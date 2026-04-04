#ifndef APPLICATION_EVENTS_H
#define APPLICATION_EVENTS_H

#include "Events/Event.h"
#include <sstream>
#include <cstdint>

namespace Cober {

    class CB_API WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height)
            : m_Width(width), m_Height(height) {}

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

    private:
        uint32_t m_Width, m_Height;
    };

    class CB_API WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        std::string ToString() const override
        {
            return "WindowCloseEvent";
        }

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class CB_API WindowMinimizedEvent : public Event
    {
    public:
        WindowMinimizedEvent() = default;
        std::string ToString() const override
        {
            return "WindowMinimizedEvent";
        }
        EVENT_CLASS_TYPE(WindowMinimized)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

     class CB_API WindowRestoredEvent : public Event
    {
    public:
        WindowRestoredEvent() = default;
        std::string ToString() const override
        {
            return "WindowRestoredEvent";
        }
        EVENT_CLASS_TYPE(WindowRestored)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class CB_API WindowFocusEvent : public Event
    {
    public:
        WindowFocusEvent() = default;
        EVENT_CLASS_TYPE(WindowFocus)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class CB_API WindowLostFocusEvent : public Event
    {
    public:
        WindowLostFocusEvent() = default;
        EVENT_CLASS_TYPE(WindowLostFocus)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

    class CB_API WindowMovedEvent : public Event
    {
    public:
        WindowMovedEvent(int x, int y)
            : m_x(x), m_y(y) {}

        int GetX() const { return m_x; }
        int GetY() const { return m_y; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowMovedEvent: " << m_x << ", " << m_y;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowMoved)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

    private:
        int m_x, m_y;
    };
}

#endif