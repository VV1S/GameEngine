#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Input.h"

struct GLFWwindow; // Forward declaration, avoids including glfw3.h here

namespace Engine {

    // Windows + GLFW input backend implementation
    class ENGINE_API WindowsInputBackend final : public InputBackend
    {
    public:
        WindowsInputBackend() = default;
        ~WindowsInputBackend() override = default;

        bool IsKeyPressed(int keycode) const override;
        bool IsMouseButtonPressed(int button) const override;
        std::pair<float, float> GetMousePosition() const override;
        float GetMouseX() const override;
        float GetMouseY() const override;

    private:
        // Helper: retrieves the GLFW window handle from Engine::Application
        GLFWwindow* GetGLFWWindow() const;
    };

} // namespace Engine
