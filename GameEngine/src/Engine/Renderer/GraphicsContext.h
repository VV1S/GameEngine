#pragma once

namespace Engine {

    // Thin interface for a graphics context (GLFW/OpenGL, etc.)
    class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };

} // namespace Engine
