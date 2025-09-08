#pragma once
#include "Engine/Core/Application.h"
#include "Engine/Events/ApplicationEvent.h"

namespace Engine {

    // "Key" to private Application methods (enable via defines: EG_TESTS)
    struct ApplicationTestAccess {
        static void HandleEvent(Application& a, Event& e) { a.HandleEvent(e); }
        static void UpdateLayers(Application& a, Timestep t) { a.UpdateLayers(t); }
        static bool CallOnWindowResize(Application& a, WindowResizeEvent& e) { return a.OnWindowResize(e); }
        static bool CallOnWindowClose(Application& a, WindowCloseEvent& e) { return a.OnWindowClose(e); }
    };

} // namespace Engine
