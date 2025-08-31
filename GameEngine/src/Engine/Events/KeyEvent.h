#pragma once

#include "Event.h"
#include <sstream>

namespace Engine {

    // Base for keyboard events
    class ENGINE_API KeyEvent : public Event
    {
    public:
        int GetKeyCode() const { return m_Key; }

        // Common group flags for all keyboard events
        uint32_t Groups() const noexcept override
        {
            return ToMask(EventGroup::Input) | ToMask(EventGroup::Keyboard);
        }

    protected:
        explicit KeyEvent(int key) : m_Key(key) {}
        int m_Key;
    };

    // Key Pressed
    class ENGINE_API KeyPressedEvent : public KeyEvent
    {
    public:
        static constexpr EventKind kKind = EventKind::KeyPressed;

        KeyPressedEvent(int key, int repeatCount)
            : KeyEvent(key), m_Repeat(repeatCount) {
        }

        int GetRepeatCount() const { return m_Repeat; }

        EventKind   Kind()   const noexcept override { return kKind; }
        const char* Name()   const noexcept override { return "KeyPressed"; }

        std::string ToString() const override
        {
            std::ostringstream ss;
            ss << Name() << ": " << m_Key << " (" << m_Repeat << " repeats)";
            return ss.str();
        }

    private:
        int m_Repeat = 0;
    };

    // Key Released
    class ENGINE_API KeyReleasedEvent : public KeyEvent
    {
    public:
        static constexpr EventKind kKind = EventKind::KeyReleased;

        explicit KeyReleasedEvent(int key) : KeyEvent(key) {}

        EventKind   Kind()   const noexcept override { return kKind; }
        const char* Name()   const noexcept override { return "KeyReleased"; }

        std::string ToString() const override
        {
            std::ostringstream ss;
            ss << Name() << ": " << m_Key;
            return ss.str();
        }
    };

    // Key Typed (text input)
    class ENGINE_API KeyTypedEvent : public KeyEvent
    {
    public:
        static constexpr EventKind kKind = EventKind::KeyTyped;

        explicit KeyTypedEvent(int key) : KeyEvent(key) {}

        EventKind   Kind()   const noexcept override { return kKind; }
        const char* Name()   const noexcept override { return "KeyTyped"; }

        std::string ToString() const override
        {
            std::ostringstream ss;
            ss << Name() << ": " << m_Key;
            return ss.str();
        }
    };

} // namespace Engine
