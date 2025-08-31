#include "enginepch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine {

    OpenGLContext::OpenGLContext(GLFWwindow* window)
        : m_Window(window) {
        EG_CORE_CHECK(m_Window, "Window handle is null!");
    }

    void OpenGLContext::Init() {
        EG_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_Window);

        const int ok = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        EG_CORE_CHECK(ok, "Failed to initialize GLAD!");

        EG_CORE_INFO("OpenGL Info:");
        EG_CORE_INFO("  Vendor:   {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        EG_CORE_INFO("  Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        EG_CORE_INFO("  Version:  {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    }

    void OpenGLContext::SwapBuffers() {
        glfwSwapBuffers(m_Window);
    }

} // namespace Engine
