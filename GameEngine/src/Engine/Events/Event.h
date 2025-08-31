#pragma once

#include <cstdint>
#include <functional>
#include <ostream>
#include <string>
#include "Engine/Core/Core.h"

namespace Engine {

    // Exact event type (kind)
    enum class EventKind : uint16_t
    {
        None = 0,

        // Window / App
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        AppUpdate,
        AppRender,

        // Keyboard
        KeyPressed,
        KeyReleased,
        KeyTyped,

        // Mouse
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    // Bitmask groups for quick filtering/routing
    enum class EventGroup : uint32_t
    {
        None = 0,
        Application = BitMask(0),
        Input = BitMask(1),
        Keyboard = BitMask(2),
        Mouse = BitMask(3),
        MouseButton = BitMask(4),
    };

    inline constexpr uint32_t ToMask(EventGroup g) { return static_cast<uint32_t>(g); }
    inline constexpr EventGroup operator|(EventGroup a, EventGroup b)
    {
        return static_cast<EventGroup>(ToMask(a) | ToMask(b));
    }

    class ENGINE_API Event
    {
    public:
        virtual ~Event() = default;

        // Identity
        virtual EventKind    Kind()   const noexcept = 0;
        virtual const char* Name()   const noexcept = 0;
        virtual uint32_t     Groups() const noexcept = 0;

        // Text
        virtual std::string ToString() const { return Name(); }

        // State
        bool Handled = false;

        // Helpers
        bool IsInGroup(EventGroup group) const noexcept
        {
            return (Groups() & ToMask(group)) != 0;
        }
    };

    // Simple type-based dispatcher
    class EventDispatcher
    {
    public:
        explicit EventDispatcher(Event& e) : m_Event(e) {}

        template<typename TEvent, typename Fn>
        bool Dispatch(Fn&& fn)
        {
            if (m_Event.Kind() == TEvent::kKind)
            {
                m_Event.Handled = std::forward<Fn>(fn)(static_cast<TEvent&>(m_Event));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }

} // namespace Engine
