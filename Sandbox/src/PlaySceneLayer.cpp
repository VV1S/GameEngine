#include "enginepch.h"
#include "PlaySceneLayer.h"

#include "Engine/Core/Application.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/RenderCommand.h"

#include "Course.h"
#include "Ship.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

PlaySceneLayer::PlaySceneLayer()
    : Layer("PlaySceneLayer")
{
    auto& win = Engine::Application::Get().GetWindow();
    RecreateCamera(win.GetWidth(), win.GetHeight());
}

PlaySceneLayer::~PlaySceneLayer() = default;

void PlaySceneLayer::OnAttach()
{
    m_Course = std::make_unique<Course>();
    m_Course->Initialize();

    // Bigger fonts for HUD
    ImGuiIO& io = ImGui::GetIO();
    m_FontTitle = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 96.0f);
    m_FontScore = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 48.0f);
}

void PlaySceneLayer::OnDetach()
{
    m_Course.reset();
}

void PlaySceneLayer::OnUpdate(Engine::Timestep dt)
{
    m_Time += dt.GetSeconds();
    m_BlinkPhase += dt.GetSeconds();

	// Camera follows player ship
    const auto& shipPos = m_Course->PlayerRef().Position();
    m_Camera->SetPosition({ shipPos.x, shipPos.y, 0.0f });

    if (m_Mode == Mode::Running)
    {
        m_Course->Tick(dt);
        if (m_Course->Ended())
            m_Mode = Mode::Defeat;
    }

    Engine::RenderCommand::SetClearColor({ 0.f, 0.f, 0.f, 1.f });
    Engine::RenderCommand::Clear();

    Engine::Renderer2D::BeginScene(*m_Camera);
    m_Course->Draw();
    Engine::Renderer2D::EndScene();
}

void PlaySceneLayer::OnImGuiRender()
{
    DrawHUD();
}

void PlaySceneLayer::DrawHUD()
{
    ImVec2 origin = ImGui::GetWindowPos();
    const auto& wnd = Engine::Application::Get().GetWindow();

    if (m_Mode == Mode::Running)
    {
        const uint32_t score = m_Course->PlayerRef().Score();
        std::string text = "Score: " + std::to_string(score);

        ImFont* f = m_FontScore ? m_FontScore : ImGui::GetFont();
        float   sz = m_FontScore ? m_FontScore->FontSize : ImGui::GetFontSize() * 2.0f;

        ImGui::GetForegroundDrawList()->AddText(
            f, sz,
            { origin.x + 16.0f, origin.y + 16.0f },
            0xffffffff, text.c_str());
        return;
    }

    // Menu / Defeat
    float blink = 0.5f + 0.5f * std::sinf(m_BlinkPhase * 3.0f);
    ImU32 color = ImGui::GetColorU32(ImVec4(1, 1, 1, blink));

    float baseY = 100.0f;
    if (m_Mode == Mode::Defeat)
        baseY = 10.0f;

    ImVec2 center = { origin.x + wnd.GetWidth() * 0.5f, origin.y + baseY };
    const char* label = (m_Mode == Mode::Defeat) ? "Click to Retry" : "Click to Start";

    ImFont* fTitle = m_FontTitle ? m_FontTitle : ImGui::GetFont();
    float   szTitle = m_FontTitle ? m_FontTitle->FontSize : ImGui::GetFontSize() * 3.0f;
    ImVec2  approxSz = ImGui::CalcTextSize(label);

    ImGui::GetForegroundDrawList()->AddText(
        fTitle, szTitle,
        { center.x - approxSz.x * 0.5f, center.y },
        color, label);

    if (m_Mode == Mode::Defeat)
    {
        const uint32_t score = m_Course->PlayerRef().Score();
        std::string text = "Score: " + std::to_string(score);

        float spacing = szTitle * 0.4f; 
        ImVec2 pos = { center.x - 80.0f, center.y + spacing };

        ImFont* f = m_FontScore ? m_FontScore : ImGui::GetFont();
        float   sz = m_FontScore ? m_FontScore->FontSize : ImGui::GetFontSize() * 2.0f;

        ImGui::GetForegroundDrawList()->AddText(
            f, sz, pos, 0xffffffff, text.c_str());
    }
}

void PlaySceneLayer::OnEvent(Engine::Event& e)
{
    Engine::EventDispatcher d(e);
    d.Dispatch<Engine::WindowResizeEvent>([this](auto& ev) { return HandleResize(ev); });
    d.Dispatch<Engine::MouseButtonPressedEvent>([this](auto& ev) { return HandleMousePress(ev); });
}

bool PlaySceneLayer::HandleResize(Engine::WindowResizeEvent& e)
{
    RecreateCamera(e.GetWidth(), e.GetHeight());
    return false;
}

bool PlaySceneLayer::HandleMousePress(Engine::MouseButtonPressedEvent&)
{
    if (m_Mode == Mode::Defeat)
        m_Course->Reset();

    if (m_Mode != Mode::Running)
        m_Mode = Mode::Running;

    return false;
}

void PlaySceneLayer::RecreateCamera(uint32_t width, uint32_t height)
{
    const float aspect = (height > 0) ? (float)width / (float)height : 16.0f / 9.0f;
    const float halfH = 8.0f;
    const float bottom = -halfH;
    const float top = halfH;
    const float left = bottom * aspect;
    const float right = top * aspect;

    m_Camera = std::make_unique<Engine::OrthographicCamera>(left, right, bottom, top);
}
