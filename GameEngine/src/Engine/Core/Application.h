#pragma once

#include "Core.h"
#include "Window.h"

#include "Engine/Core/LayerStack.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Core/Timestep.h"
#include "Engine/ImGui/ImGuiLayer.h"

#include <memory>

namespace Engine {

    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        // Layer management
        void AddLayer(const std::shared_ptr<Layer>& layer);
        void AddOverlay(const std::shared_ptr<Layer>& overlay);

        // Accessors
        inline Window& GetWindow() { return *m_Window; }
        inline static Application& Get() { return *s_Instance; }

    private:
        // Event pipeline
        void HandleEvent(Event& e);
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        // Main loop helpers
        void UpdateLayers(Timestep dt);
        void RenderImGui();

    private:
        std::unique_ptr<Window>    m_Window;
        std::shared_ptr<ImGuiLayer> m_ImGuiLayer;

        bool   m_IsRunning = false;
        bool   m_Minimized = false;
        float  m_LastFrameTime = 0.0f;
        LayerStack m_LayerStack;

        static Application* s_Instance;
    };

    // Implemented in the client (Sandbox)
    Application* CreateApplication();

} // namespace Engine
