#pragma once
#include <functional>
#include "Engine/Renderer/Shader.h"

class PongUI {
public:
    PongUI(Engine::Shader* shader,
        float& speed, float& tiling, float& saturation, float& value, float& alpha,
        int& scoreL, int& scoreR,
        std::function<void()> onReset);

    // Rysuje okno ImGui (domyœlnie „Pong”)
    void Draw(const char* windowName = "Pong");

private:
    Engine::Shader* m_Shader; // nie posiada; zarz¹dza nim PongLayer

    // Edytowane parametry shadera (referencje do danych trzymanych w PongLayer)
    float& m_Speed;
    float& m_Tiling;
    float& m_Saturation;
    float& m_Value;
    float& m_Alpha;

    // Wynik / akcje gry
    int& m_ScoreL;
    int& m_ScoreR;
    std::function<void()> m_OnReset;
};
