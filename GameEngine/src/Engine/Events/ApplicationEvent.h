#pragma once

#include "Event.h"
#include <sstream>

namespace Engine {

    // ------------------------------
    // Window Resize
    // ------------------------------
    class ENGINE_API WindowResizeEvent : public Event
    {
    public:
        static constexpr EventKind kKind = EventKind::WindowResize;

        WindowResizeEvent(uint32_t w, uint32_t h) : m_Width(w), m_Height(h) {}

        uint32_t GetWidth()  const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

        // Event interface
        EventKind   Kind()   const noexcept override { return kKind; }
        const char* Name()   const noexcept override { return "WindowResize"; }
        uint32_t    Groups() const noexcept override { return ToMask(EventGroup::Application); }

        std::string ToString() const override
        {
            std::ostringstream ss;
            ss << Name() << ": " << m_Width << "x" << m_Height;
            return ss.str();
        }

    private:
        uint32_t m_Width, m_Height;
    };

    // ------------------------------
    // Window Close
    // ------------------------------
    class ENGINE_API WindowCloseEvent : public Event
    {
    public:
        static constexpr EventKind kKind = EventKind::WindowClose;

        EventKind   Kind()   const noexcept override { return kKind; }
        const char* Name()   const noexcept override { return "WindowClose"; }
        uint32_t    Groups() const noexcept override { return ToMask(EventGroup::Application); }
    };

    // ------------------------------
    // App Tick
    // ------------------------------
    class ENGINE_API AppTickEvent : public Event
    {
    public:
        static constexpr EventKind kKind = EventKind::AppTick;

        EventKind   Kind()   const noexcept override { return kKind; }
        const char* Name()   const noexcept override { return "AppTick"; }
        uint32_t    Groups() const noexcept override { return ToMask(EventGroup::Application); }
    };

    // ------------------------------
    // App Update
    // ------------------------------
    class ENGINE_API AppUpdateEvent : public Event
    {
    public:
        static constexpr EventKind kKind = EventKind::AppUpdate;

        EventKind   Kind()   const noexcept override { return kKind; }
        const char* Name()   const noexcept override { return "AppUpdate"; }
        uint32_t    Groups() const noexcept override { return ToMask(EventGroup::Application); }
    };

    // ------------------------------
    // App Render
    // ------------------------------
    class ENGINE_API AppRenderEvent : public Event
    {
    public:
        static constexpr EventKind kKind = EventKind::AppRender;

        EventKind   Kind()   const noexcept override { return kKind; }
        const char* Name()   const noexcept override { return "AppRender"; }
        uint32_t    Groups() const noexcept override { return ToMask(EventGroup::Application); }
    };

} // namespace Engine
