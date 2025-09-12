#include "Paddles.h"
#include <glm/common.hpp>
#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"

Paddles::Paddles(
    const glm::vec2& paddleSize,
    float paddleSpeed,
    float halfWorldHeight,
    float margin,
    float leftX,
    float rightX
)
    : m_PaddleSize(paddleSize)
    , m_PaddleSpeed(paddleSpeed)
    , m_HalfWorldHeight(halfWorldHeight)
    , m_Margin(margin)
    , m_LeftPos(leftX, 0.0f)
    , m_RightPos(rightX, 0.0f)
{
}

void Paddles::ResetY()
{
    m_LeftPos.y = 0.0f;
    m_RightPos.y = 0.0f;
}

void Paddles::ClampToBounds()
{
    const float limit = m_HalfWorldHeight - m_PaddleSize.y * 0.5f - m_Margin;
    m_LeftPos.y  = glm::clamp(m_LeftPos.y,  -limit, +limit);
    m_RightPos.y = glm::clamp(m_RightPos.y, -limit, +limit);
}

void Paddles::Update(Engine::Timestep dt)
{
    float dyL = 0.0f, dyR = 0.0f;

    if (Engine::Input::IsKeyPressed(EG_KEY_W)) dyL += m_PaddleSpeed;
    if (Engine::Input::IsKeyPressed(EG_KEY_S)) dyL -= m_PaddleSpeed;

    if (Engine::Input::IsKeyPressed(EG_KEY_UP))   dyR += m_PaddleSpeed;
    if (Engine::Input::IsKeyPressed(EG_KEY_DOWN)) dyR -= m_PaddleSpeed;

    const float ds = dt.GetSeconds();
    m_LeftPos.y  += dyL * ds;
    m_RightPos.y += dyR * ds;

    ClampToBounds();
}

bool Paddles::AABB(const glm::vec2& p0, const glm::vec2& s0,
                   const glm::vec2& p1, const glm::vec2& s1)
{
    const glm::vec2 h0 = s0 * 0.5f;
    const glm::vec2 h1 = s1 * 0.5f;
    const bool x = std::abs(p0.x - p1.x) <= (h0.x + h1.x);
    const bool y = std::abs(p0.y - p1.y) <= (h0.y + h1.y);
    return x && y;
}

bool Paddles::BounceBallIfNeeded(glm::vec2& ballPos, glm::vec2& ballVel, const glm::vec2& ballSize)
{
    bool bounced = false;

    // lewa
    if (ballVel.x < 0.0f && AABB(ballPos, ballSize, m_LeftPos, m_PaddleSize))
    {
        ballPos.x = m_LeftPos.x + (m_PaddleSize.x + ballSize.x) * 0.5f + 0.01f;
        ballVel.x *= -1.0f;

        const float off = (ballPos.y - m_LeftPos.y) / (m_PaddleSize.y * 0.5f);
        ballVel.y += off * 4.5f;

        bounced = true;
    }
    // prawa
    if (ballVel.x > 0.0f && AABB(ballPos, ballSize, m_RightPos, m_PaddleSize))
    {
        ballPos.x = m_RightPos.x - (m_PaddleSize.x + ballSize.x) * 0.5f - 0.01f;
        ballVel.x *= -1.0f;

        const float off = (ballPos.y - m_RightPos.y) / (m_PaddleSize.y * 0.5f);
        ballVel.y += off * 4.5f;

        bounced = true;
    }

    return bounced;
}
