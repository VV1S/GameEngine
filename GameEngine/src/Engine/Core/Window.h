#pragma once

#include <functional>
#include <string>
#include <cstdint>

#include "Engine/Core/Core.h"
#include "Engine/Events/Event.h"

namespace Engine {

    // Strongly-typed window configuration with a few common flags.
    struct WindowConfig
    {
        std::string Title = "Game Engine";
        uint32_t    Width = 1280;
        uint32_t    Height = 720;
        bool        VSync = true;
        bool        Resizable = true;
        bool        Visible = true;
    };

    // Cross-platform window interface.
    // NOTE: Lifecycle (create/destroy) handled via Unique<Window>.
    class ENGINE_API Window
    {
    public:
        using EventCallback = std::function<void(Event&)>;

        virtual ~Window() = default;

        // Per-frame pump: usually polls events & swaps buffers.
        virtual void OnUpdate() = 0;

        // Dimensions (window size in screen coordinates)
        virtual uint32_t GetWidth()  const = 0;
        virtual uint32_t GetHeight() const = 0;

        // Optional: framebuffer size (pixels), useful on HiDPI
        virtual void GetFramebufferSize(uint32_t& outW, uint32_t& outH) const { outW = GetWidth(); outH = GetHeight(); }

        // DPI scale (1.0 = 96 DPI); default 1.0 if unsupported
        virtual float GetDPIScale() const { return 1.0f; }

        // Attributes
        virtual void SetTitle(const std::string& title) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        // Cursor / clipboard conveniences (optional; provide sensible defaults)
        virtual void  SetCursorVisible(bool visible) {}
        virtual void  SetClipboardText(const std::string& text) {}
        virtual std::string GetClipboardText() const { return {}; }

        // Event hookup
        virtual void SetEventCallback(const EventCallback& callback) = 0;

        // Native handle (GLFWwindow*, HWND, etc.)
        virtual void* GetNativeWindow() const = 0;

        // Factory (implemented per-platform)
        static Unique<Window> Create(const WindowConfig& cfg = {});
    };

} // namespace Engine
