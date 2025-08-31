#include "enginepch.h"
#include "WindowsInput.h"

#include "Engine/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Engine {

    GLFWwindow* WindowsInputBackend::GetGLFWWindow() const
    {
        // Application owns the Window instance -> get native GLFW pointer
        return static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    }

    bool WindowsInputBackend::IsKeyPressed(int keycode) const
    {
        if (auto* window = GetGLFWWindow())
        {
            int state = glfwGetKey(window, keycode);
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }
        return false;
    }

    bool WindowsInputBackend::IsMouseButtonPressed(int button) const
    {
        if (auto* window = GetGLFWWindow())
        {
            int state = glfwGetMouseButton(window, button);
            return state == GLFW_PRESS;
        }
        return false;
    }

    std::pair<float, float> WindowsInputBackend::GetMousePosition() const
    {
        double x = 0.0, y = 0.0;
        if (auto* window = GetGLFWWindow())
            glfwGetCursorPos(window, &x, &y);
        return { static_cast<float>(x), static_cast<float>(y) };
    }

    float WindowsInputBackend::GetMouseX() const
    {
        auto [x, _] = GetMousePosition();
        return x;
    }

    float WindowsInputBackend::GetMouseY() const
    {
        auto [_, y] = GetMousePosition();
        return y;
    }

} // namespace Engine
