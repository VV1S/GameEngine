#include "enginepch.h"
#include "GameLayer.h"

#include "Engine/Core/Application.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "Random.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Engine;

GameLayer::GameLayer()
    : Layer("GameLayer")
{
    auto& window = Application::Get().GetWindow();
    CreateCamera(window.GetWidth(), window.GetHeight());
    Random::Init();
}

void GameLayer::OnAttach()
{
    m_Level.Init();

    ImGuiIO& io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 120.0f);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Engine::Timestep ts)
{
    m_Time += ts.GetSeconds();
    if ((int)(m_Time * 10.0f) % 8 > 4)
        m_Blink = !m_Blink;

    if (m_Level.IsGameOver())
        m_State = GameState::GameOver;

    const auto& playerPos = m_Level.GetPlayer().GetPosition();
    m_Camera->SetPosition({ playerPos.x, playerPos.y, 0.0f });

    switch (m_State)
    {
    case GameState::Play:
        m_Level.OnUpdate(ts);
        break;
    default:
        break;
    }

    RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    RenderCommand::Clear();

    Renderer2D::BeginScene(*m_Camera);
    m_Level.OnRender();
    Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
    switch (m_State)
    {
    case GameState::Play:
    {
        const uint32_t score = m_Level.GetPlayer().GetScore();
        const std::string text = "Score: " + std::to_string(score);
        ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, text.c_str());
        break;
    }
    case GameState::MainMenu:
    case GameState::GameOver:
    {
        ImVec2 pos = ImGui::GetWindowPos();
        const auto& win = Application::Get().GetWindow();
        pos.x += win.GetWidth() * 0.5f - 300.0f;
        pos.y += 50.0f;
        if (m_Blink)
            ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");

        if (m_State == GameState::GameOver)
        {
            ImVec2 pos2 = pos;
            pos2.x += 200.0f;
            pos2.y += 150.0f;
            const uint32_t score = m_Level.GetPlayer().GetScore();
            const std::string text = "Score: " + std::to_string(score);
            ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos2, 0xffffffff, text.c_str());
        }
        break;
    }
    }
}

void GameLayer::OnEvent(Engine::Event& e)
{
    EventDispatcher dispatcher(e);

    dispatcher.Dispatch<WindowResizeEvent>(
        [this](WindowResizeEvent& ev) -> bool { return OnWindowResize(ev); });

    dispatcher.Dispatch<MouseButtonPressedEvent>(
        [this](MouseButtonPressedEvent& ev) -> bool { return OnMouseButtonPressed(ev); });
}

bool GameLayer::OnMouseButtonPressed(Engine::MouseButtonPressedEvent& e)
{
    (void)e; // unused for now
    if (m_State == GameState::GameOver)
        m_Level.Reset();

    m_State = GameState::Play;
    return false;
}

bool GameLayer::OnWindowResize(Engine::WindowResizeEvent& e)
{
    CreateCamera(e.GetWidth(), e.GetHeight());
    return false;
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
    const float aspect = (height > 0) ? (float)width / (float)height : 16.0f / 9.0f;

    const float camHalfHeight = 8.0f;          // tune this to “zoom out”
    const float bottom = -camHalfHeight;
    const float top = camHalfHeight;
    const float left = bottom * aspect;
    const float right = top * aspect;

    m_Camera = std::make_unique<OrthographicCamera>(left, right, bottom, top);
}
