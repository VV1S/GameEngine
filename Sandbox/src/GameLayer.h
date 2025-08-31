#pragma once

#include "Engine/Core/Timestep.h"
#include "Engine/Core/Layer.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "Level.h"
#include <imgui/imgui.h>
#include <memory>

class GameLayer : public Engine::Layer
{
public:
    GameLayer();
    ~GameLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Engine::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Engine::Event& e) override;

private:
    bool OnMouseButtonPressed(Engine::MouseButtonPressedEvent& e);
    bool OnWindowResize(Engine::WindowResizeEvent& e);
    void CreateCamera(uint32_t width, uint32_t height);

private:
    std::unique_ptr<Engine::OrthographicCamera> m_Camera;
    Level  m_Level;
    ImFont* m_Font = nullptr;

    float m_Time = 0.0f;
    bool  m_Blink = false;

    enum class GameState { Play = 0, MainMenu = 1, GameOver = 2 };
    GameState m_State = GameState::MainMenu;
};
