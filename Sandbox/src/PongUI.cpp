#include "PongUI.h"
#include <imgui/imgui.h>

PongUI::PongUI(Engine::Shader* shader,
    float& speed, float& tiling, float& saturation, float& value, float& alpha,
    int& scoreL, int& scoreR,
    std::function<void()> onReset)
    : m_Shader(shader)
    , m_Speed(speed), m_Tiling(tiling), m_Saturation(saturation), m_Value(value), m_Alpha(alpha)
    , m_ScoreL(scoreL), m_ScoreR(scoreR)
    , m_OnReset(std::move(onReset))
{
}

void PongUI::Draw(const char* windowName)
{
    ImGuiViewport* vp = ImGui::GetMainViewport();

    // Position: center of the width, at the top of the work area (with a small margin)
    constexpr float kTopMargin = 12.0f;
    const ImVec2 pos(
        vp->WorkPos.x + vp->WorkSize.x * 0.5f,  // center horizontally
        vp->WorkPos.y + kTopMargin               // just below the top edge
    );

    // Set position with pivot (0.5, 0.0): window reference point = center of the top edge
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.85f); // slightly transparent background

    // Lightweight, "floating" bar without borders and without docking
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoDocking;

    if (!ImGui::Begin(windowName, nullptr, flags)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Score:  %d : %d", m_ScoreL, m_ScoreR);
    ImGui::Separator();

    ImGui::TextUnformatted("Rainbow shader");
    bool refresh = false;
    refresh |= ImGui::SliderFloat("Speed", &m_Speed, 0.0f, 3.0f);
    refresh |= ImGui::SliderFloat("Tiling", &m_Tiling, 0.2f, 6.0f);
    refresh |= ImGui::SliderFloat("Saturation", &m_Saturation, 0.0f, 1.0f);
    refresh |= ImGui::SliderFloat("Value", &m_Value, 0.0f, 2.0f);
    refresh |= ImGui::SliderFloat("Alpha", &m_Alpha, 0.0f, 1.0f);

    if (refresh && m_Shader) {
        m_Shader->Bind();
        m_Shader->SetFloat("u_Saturation", m_Saturation);
        m_Shader->SetFloat("u_Value", m_Value);
        m_Shader->SetFloat("u_Alpha", m_Alpha);
        m_Shader->SetFloat("u_Speed", m_Speed);
        m_Shader->SetFloat("u_Tiling", m_Tiling);
        m_Shader->Unbinding();
    }

    ImGui::Separator();
    if (ImGui::Button("Reset round") && m_OnReset) {
        m_OnReset();
    }

    ImGui::End();
}
