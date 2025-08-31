#pragma once

#include "Engine/Core/Window.h"
#include "Engine/Renderer/GraphicsContext.h"
#include <memory>

// Forward declare to avoid including glfw everywhere in headers
struct GLFWwindow;

namespace Engine {

    class WindowsWindow final : public Window
    {
    public:
        explicit WindowsWindow(const WindowConfig& cfg);
        ~WindowsWindow() override;

        // Per-frame pump
        void OnUpdate() override;

        // Dimensions
        uint32_t GetWidth()  const override { return m_Data.Width; }
        uint32_t GetHeight() const override { return m_Data.Height; }

        // Attributes
        void SetTitle(const std::string& title) override;
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void SetEventCallback(const EventCallback& callback) override;

        // Native handle
        void* GetNativeWindow() const override { return m_Window; }

    private:
        void Init(const WindowConfig& cfg);
        void Shutdown();

    private:
        GLFWwindow* m_Window = nullptr;
        Unique<GraphicsContext> m_Context;

        struct WindowData
        {
            std::string Title;
            uint32_t Width = 0;
            uint32_t Height = 0;
            bool VSync = true;

            EventCallback Callback; // event sink provided by Application
        } m_Data;
    };

} // namespace Engine
