#include "enginepch.h"
#include "Application.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/Input.h"
#include "../../Platforms/Windows/WindowsInput.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace Engine {

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        EG_CORE_CHECK(!s_Instance, "Application already exists!");
        s_Instance = this;

        // Create main window
        m_Window = Window::Create();
        m_Window->SetEventCallback([this](Event& e) { HandleEvent(e); });

        // Initialize input AFTER the window exists
        if (!Input::IsInitialized())
        {
            Input::Initialize(MakeUnique<WindowsInputBackend>());
        }

        Renderer::Init();

        // Setup ImGui overlay
        m_ImGuiLayer = std::make_shared<ImGuiLayer>();
        AddOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        Renderer::Shutdown();

        if (Input::IsInitialized())
            Input::Shutdown();

        s_Instance = nullptr;
    }

    void Application::AddLayer(const std::shared_ptr<Layer>& layer)
    {
        m_LayerStack.AddLayer(layer);
        layer->OnAttach();
    }

    void Application::AddOverlay(const std::shared_ptr<Layer>& overlay)
    {
        m_LayerStack.AddOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::HandleEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(
            [this](WindowCloseEvent& ev) { return OnWindowClose(ev); });
        dispatcher.Dispatch<WindowResizeEvent>(
            [this](WindowResizeEvent& ev) { return OnWindowResize(ev); });

        // Propagate to enabled layers in reverse order
        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            if ((*it)->IsEnabled())
            {
                (*it)->OnEvent(e);
                if (e.Handled) break;
            }
        }
    }

    void Application::Run()
    {
        m_IsRunning = true;

        while (m_IsRunning)
        {
            const float currentTime = static_cast<float>(glfwGetTime());
            Timestep deltaTime = currentTime - m_LastFrameTime;
            m_LastFrameTime = currentTime;

            if (!m_Minimized)
            {
                UpdateLayers(deltaTime);
                RenderImGui();
            }

            m_Window->OnUpdate();
        }
    }

    void Application::UpdateLayers(Timestep dt)
    {
        for (auto& layer : m_LayerStack)
        {
            if (layer->IsEnabled())
                layer->OnUpdate(dt);
        }
    }

    void Application::RenderImGui()
    {
        m_ImGuiLayer->Begin();
        for (auto& layer : m_LayerStack)
        {
            if (layer->IsEnabled())
                layer->OnImGuiRender();
        }
        m_ImGuiLayer->End();
    }

    bool Application::OnWindowClose(WindowCloseEvent&)
    {
        m_IsRunning = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }

} // namespace Engine
