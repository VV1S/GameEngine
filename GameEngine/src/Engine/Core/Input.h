#pragma once

#include "Core.h"
#include <utility>

namespace Engine {

    // Abstract backend interface for input handling.
    // Different platforms (Windows, Linux, etc.) should implement this.
    class ENGINE_API InputBackend {
    public:
        virtual ~InputBackend() = default;

        virtual bool IsKeyPressed(int keycode) const = 0;
        virtual bool IsMouseButtonPressed(int button) const = 0;
        virtual std::pair<float, float> GetMousePosition() const = 0;
        virtual float GetMouseX() const = 0;
        virtual float GetMouseY() const = 0;
    };

    // Static facade for input queries.
    // Delegates calls to the currently active backend.
    class ENGINE_API Input final {
    public:
        // Must be called once, after the window system has been created.
        // Takes ownership of the backend instance.
        static void Initialize(Unique<InputBackend> backend);

        // Clean up and reset the input system.
        static void Shutdown();

        // Query functions
        static bool IsKeyPressed(int keycode);
        static bool IsMouseButtonPressed(int button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();

        // Check if input has been initialized.
        static bool IsInitialized();

    private:
        // Backend ownership and raw pointer for quick access.
        static Unique<InputBackend> s_BackendOwner;
        static InputBackend* s_Backend;
    };

} // namespace Engine
