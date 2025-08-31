#include "enginepch.h"
#include "Input.h"

namespace Engine {

    // Static members
    Unique<InputBackend> Input::s_BackendOwner = nullptr;
    InputBackend* Input::s_Backend = nullptr;

    void Input::Initialize(Unique<InputBackend> backend)
    {
        EG_CORE_CHECK(!s_Backend, "Input already initialized!");
        EG_CORE_CHECK(backend != nullptr, "Input backend must not be null!");

        s_BackendOwner = std::move(backend);
        s_Backend = s_BackendOwner.get();
    }

    void Input::Shutdown()
    {
        s_Backend = nullptr;
        s_BackendOwner.reset();
    }

    bool Input::IsInitialized()
    {
        return s_Backend != nullptr;
    }

    bool Input::IsKeyPressed(int keycode)
    {
        EG_CORE_CHECK(s_Backend, "Input not initialized!");
        return s_Backend->IsKeyPressed(keycode);
    }

    bool Input::IsMouseButtonPressed(int button)
    {
        EG_CORE_CHECK(s_Backend, "Input not initialized!");
        return s_Backend->IsMouseButtonPressed(button);
    }

    std::pair<float, float> Input::GetMousePosition()
    {
        EG_CORE_CHECK(s_Backend, "Input not initialized!");
        return s_Backend->GetMousePosition();
    }

    float Input::GetMouseX()
    {
        EG_CORE_CHECK(s_Backend, "Input not initialized!");
        return s_Backend->GetMouseX();
    }

    float Input::GetMouseY()
    {
        EG_CORE_CHECK(s_Backend, "Input not initialized!");
        return s_Backend->GetMouseY();
    }

} // namespace Engine
