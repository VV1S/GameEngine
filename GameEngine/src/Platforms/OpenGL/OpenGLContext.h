#pragma once
#include "Engine/Renderer/GraphicsContext.h"
struct GLFWwindow;

namespace Engine {

    class OpenGLContext final : public GraphicsContext {
    public:
        explicit OpenGLContext(GLFWwindow* window);
        ~OpenGLContext() override = default;

        void Init() override;
        void SwapBuffers() override;

    private:
        GLFWwindow* m_Window = nullptr;
    };

} // namespace Engine
