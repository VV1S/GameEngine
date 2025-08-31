#pragma once

#include "Event.h"
#include <sstream>

namespace Engine {

    // Mouse Moved
    class ENGINE_API MouseMovedEvent : public Event
    {
    public:
        static constexpr EventKind kKind = EventKind::MouseMoved;

        MouseMovedEvent(float x, float y) : m_X(x), m_Y(y) {}

        float GetX() const { return m_X; }
        float GetY() const { return m_Y; }

        EventKind   Kind()   const noexcept override { return kKind; }
        const char* Name()   const noexcept override { return "MouseMoved"; }
        uint32_t    Groups() const noexcept override
        {
            return ToMask(EventGroup::Input) | ToMask(EventGroup::Mouse);
        }

        std::string ToString() const override
        {
            std::ostringstream ss;
            ss << Name() << ": " << m_X << ", " << m_Y;
            return ss.str();
        }

    private:
        float m_X, m_Y;
    };

    // Mouse Scrolled
    class ENGINE_API MouseScrolledEvent : public Event
    {
    public:
        static constexpr EventKind kKind = EventKind::MouseScrolled;

        MouseScrolledEvent(float xOff, float yOff) : m_XOff(xOff), m_YOff(yOff) {}

        float GetXOffset() const { return m_XOff; }
        float GetYOffset() const { return m_YOff; }

        EventKind   Kind()   const noexcept override { return kKind; }
        const char* Name()   const noexcept override { return "MouseScrolled"; }
        uint32_t    Groups() const noexcept override
        {
            return ToMask(EventGroup::Input) | ToMask(EventGroup::Mouse);
        }

        std::string ToString() const override
        {
            std::ostringstream ss;
            ss << Name() << ": " << m_XOff << ", " << m_YOff;
            return ss.str();
        }

    private:
        float m_XOff, m_YOff;
    };

    // Base Mouse Button
    class ENGINE_API MouseButtonEvent : public Event
    {
    public:
        int GetMouseButton() const { return m_Button; }

        uint32_t Groups() const noexcept override
        {
            return ToMask(EventGroup::Input) | ToMask(EventGroup::Mouse) | ToMask(EventGroup::MouseButton);
        }

    protected:
        explicit MouseButtonEvent(int button) : m_Button(button) {}
        int m_Button;
    };

    // Mouse Button Pressed
    class ENGINE_API MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        static constexpr EventKind kKind = EventKind::MouseButtonPressed;

        explicit MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

        EventKind   Kind()   const noexcept override { return kKind; }
        const char* Name()   const noexcept override { return "MouseButtonPressed"; }

        std::string ToString() const override
        {
            std::ostringstream ss;
            ss << Name() << ": " << m_Button;
            return ss.str();
        }
    };

    // Mouse Button Released
    class ENGINE_API MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        static constexpr EventKind kKind = EventKind::MouseButtonReleased;

        explicit MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

        EventKind   Kind()   const noexcept override { return kKind; }
        const char* Name()   const noexcept override { return "MouseButtonReleased"; }

        std::string ToString() const override
        {
            std::ostringstream ss;
            ss << Name() << ": " << m_Button;
            return ss.str();
        }
    };

} // namespace Engine
