#pragma once
#include <glm/glm.hpp>
#include "Engine/Core/Timestep.h"

// Reprezentuje obie paletki: stan, parametry, sterowanie i logikê odbicia pi³ki.
class Paddles
{
public:
    Paddles(
        const glm::vec2& paddleSize,   // rozmiar obu paletek
        float paddleSpeed,             // prêdkoœæ ruchu (|dy/dt|)
        float halfWorldHeight,         // po³owa wysokoœci œwiata (do clamp)
        float margin = 0.2f,         // luz od krawêdzi
        float leftX = -12.0f,       // sta³e X lewej paletki
        float rightX = +12.0f        // sta³e X prawej paletki
    );

    // Aktualizacja sterowania (W/S i ?/?) + clamp do granic.
    void Update(Engine::Timestep dt);

    // Zeruje pionowe pozycje obu paletek (u¿ywane przy starcie rundy).
    void ResetY();

    // Ewentualna zmiana parametrów œwiata/rozmiaru/szybkoœci w locie.
    void SetHalfWorldHeight(float h) { m_HalfWorldHeight = h; }
    void SetPaddleSize(const glm::vec2& s) { m_PaddleSize = s; }
    void SetSpeed(float s) { m_PaddleSpeed = s; }

    // Kolizja z pi³k¹: odbija jeœli trzeba i modyfikuje pos/vel pi³ki.
    // Zwraca true, jeœli nast¹pi³o odbicie.
    bool BounceBallIfNeeded(glm::vec2& ballPos, glm::vec2& ballVel, const glm::vec2& ballSize);

    // Dostêpy do stanu:
    const glm::vec2& GetLeftPos()  const { return m_LeftPos; }
    const glm::vec2& GetRightPos() const { return m_RightPos; }
    const glm::vec2& GetSize()     const { return m_PaddleSize; }

private:
    static bool AABB(const glm::vec2& p0, const glm::vec2& s0,
        const glm::vec2& p1, const glm::vec2& s1);
    void ClampToBounds();

private:
    glm::vec2 m_PaddleSize{ 0.6f, 2.8f };
    float     m_PaddleSpeed = 14.0f;
    float     m_HalfWorldHeight = 8.0f;
    float     m_Margin = 0.2f;

    glm::vec2 m_LeftPos{ -12.0f, 0.0f };
    glm::vec2 m_RightPos{ +12.0f, 0.0f };
};

