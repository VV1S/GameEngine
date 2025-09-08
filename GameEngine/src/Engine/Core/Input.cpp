#include "enginepch.h"
#include "Input.h"

namespace Engine {

    // Static members
    Unique<InputBackend> Input::s_BackendOwner = nullptr;
    InputBackend* Input::s_Backend = nullptr;

    void Input::Initialize(Unique<InputBackend> backend)
    {
#ifdef EG_TESTS
        if (s_Backend) {
            Shutdown();
        }
#else
        EG_CORE_CHECK(!s_Backend, "Input already initialized!");
#endif
        EG_CORE_CHECK(backend != nullptr, "Input backend must not be null!");

        s_BackendOwner = std::move(backend);
        s_Backend = s_BackendOwner.get();
    }

    void Input::Shutdown() noexcept
    {
        // Jeœli ju¿ wy³¹czone, nic nie rób
        if (!s_BackendOwner && !s_Backend)
            return;

        // Usuñ surowy wskaŸnik jako pierwszy
        s_Backend = nullptr;

        // Wyjmij unikatowy wskaŸnik do zmiennej lokalnej i zniszcz go poza statycznym stanem
        auto owned = std::move(s_BackendOwner);
        // owned zniknie po wyjœciu z funkcji -> bezpieczniej wzglêdem destruktorów backendu
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
