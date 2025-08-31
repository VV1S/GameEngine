#include "enginepch.h"
#include "WindowsWindow.h"

#include "Engine/Core/Log.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"

#include <GLFW/glfw3.h>
#include "Platforms/OpenGL/OpenGLContext.h"

namespace Engine {

    // Single init guard for GLFW
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        EG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    // Factory (Window.h declares: static Unique<Window> Create(const WindowConfig&))
    Unique<Window> Window::Create(const WindowConfig& cfg)
    {
        return MakeUnique<WindowsWindow>(cfg);
    }

    WindowsWindow::WindowsWindow(const WindowConfig& cfg)
    {
        EG_PROFILE_FUNCTION();
        Init(cfg);
    }

    WindowsWindow::~WindowsWindow()
    {
        EG_PROFILE_FUNCTION();
        Shutdown();
    }

    void WindowsWindow::Init(const WindowConfig& cfg)
    {
        EG_PROFILE_FUNCTION();

        m_Data.Title = cfg.Title;
        m_Data.Width = cfg.Width;
        m_Data.Height = cfg.Height;
        m_Data.VSync = cfg.VSync;

        EG_CORE_INFO("Creating window {0} ({1} x {2})", m_Data.Title, m_Data.Width, m_Data.Height);

        if (!s_GLFWInitialized)
        {
            EG_PROFILE_SCOPE("glfwInit");

            if (!glfwInit())
                EG_CORE_CHECK(false, "Could not initialize GLFW!");

            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        // Window hints (resizable/visible) based on cfg
        glfwWindowHint(GLFW_RESIZABLE, cfg.Resizable ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_VISIBLE, cfg.Visible ? GLFW_TRUE : GLFW_FALSE);

        {
            EG_PROFILE_SCOPE("glfwCreateWindow");
            m_Window = glfwCreateWindow(
                static_cast<int>(m_Data.Width),
                static_cast<int>(m_Data.Height),
                m_Data.Title.c_str(),
                nullptr, nullptr
            );
        }

        EG_CORE_CHECK(m_Window != nullptr, "glfwCreateWindow failed!");

        // Create graphics context (OpenGL here)
        m_Context = MakeUnique<OpenGLContext>(m_Window);
        m_Context->Init();

        // Store pointer to our data for callbacks
        glfwSetWindowUserPointer(m_Window, &m_Data);

        // VSync
        SetVSync(m_Data.VSync);

        // ---- GLFW callbacks ----

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                data.Width = static_cast<uint32_t>(width);
                data.Height = static_cast<uint32_t>(height);

                if (data.Callback)
                {
                    WindowResizeEvent event(data.Width, data.Height);
                    data.Callback(event);
                }
            });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                if (data.Callback)
                {
                    WindowCloseEvent event;
                    data.Callback(event);
                }
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                if (!data.Callback) return;

                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.Callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.Callback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.Callback(event);
                    break;
                }
                }
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint)
            {
                auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                if (!data.Callback) return;

                KeyTypedEvent event(static_cast<int>(codepoint));
                data.Callback(event);
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                if (!data.Callback) return;

                if (action == GLFW_PRESS)
                {
                    MouseButtonPressedEvent event(button);
                    data.Callback(event);
                }
                else if (action == GLFW_RELEASE)
                {
                    MouseButtonReleasedEvent event(button);
                    data.Callback(event);
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                if (!data.Callback) return;

                MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
                data.Callback(event);
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                if (!data.Callback) return;

                MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
                data.Callback(event);
            });
    }

    void WindowsWindow::Shutdown()
    {
        EG_PROFILE_FUNCTION();

        if (m_Window)
        {
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;
        }

        // Context is unique_ptr and will free automatically.
        m_Context.reset();
    }

    void WindowsWindow::OnUpdate()
    {
        EG_PROFILE_FUNCTION();

        glfwPollEvents();
        if (m_Context)
            m_Context->SwapBuffers();
    }

    void WindowsWindow::SetTitle(const std::string& title)
    {
        m_Data.Title = title;
        if (m_Window)
            glfwSetWindowTitle(m_Window, m_Data.Title.c_str());
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        EG_PROFILE_FUNCTION();
        glfwSwapInterval(enabled ? 1 : 0);
        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

    void WindowsWindow::SetEventCallback(const EventCallback& callback)
    {
        m_Data.Callback = callback;
    }

} // namespace Engine
