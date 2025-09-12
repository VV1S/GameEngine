#pragma once
#include <glm/glm.hpp>
#include "Engine/Core/Timestep.h"

// Reprezentuje obie paletki: stan, parametry, sterowanie i logik� odbicia pi�ki.
class Paddles
{
public:
    Paddles(
        const glm::vec2& paddleSize,   // rozmiar obu paletek
        float paddleSpeed,             // pr�dko�� ruchu (|dy/dt|)
        float halfWorldHeight,         // po�owa wysoko�ci �wiata (do clamp)
        float margin = 0.2f,         // luz od kraw�dzi
        float leftX = -12.0f,       // sta�e X lewej paletki
        float rightX = +12.0f        // sta�e X prawej paletki
    );

    // Aktualizacja sterowania (W/S i ?/?) + clamp do granic.
    void Update(Engine::Timestep dt);

    // Zeruje pionowe pozycje obu paletek (u�ywane przy starcie rundy).
    void ResetY();

    // Ewentualna zmiana parametr�w �wiata/rozmiaru/szybko�ci w locie.
    void SetHalfWorldHeight(float h) { m_HalfWorldHeight = h; }
    void SetPaddleSize(const glm::vec2& s) { m_PaddleSize = s; }
    void SetSpeed(float s) { m_PaddleSpeed = s; }

    // Kolizja z pi�k�: odbija je�li trzeba i modyfikuje pos/vel pi�ki.
    // Zwraca true, je�li nast�pi�o odbicie.
    bool BounceBallIfNeeded(glm::vec2& ballPos, glm::vec2& ballVel, const glm::vec2& ballSize);

    // Dost�py do stanu:
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

