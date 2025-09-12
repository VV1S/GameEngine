#include "PongLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include "Engine/Core/Application.h"
#include "PongUI.h"

PongLayer::PongLayer()
    : Layer("PongLayer")
    , m_Paddles({ 0.6f, 2.8f }, 14.0f, m_HalfWorldHeight, 0.2f, -12.0f, +12.0f)
{
    auto& win = Engine::Application::Get().GetWindow();
    RecreateCamera(win.GetWidth(), win.GetHeight());
}

PongLayer::~PongLayer() = default;

void PongLayer::OnAttach()
{
    // shader têczowy do paletek
    m_RainbowShader = Engine::Shader::Create("assets/shaders/Rainbow.glsl");

    // uniwersalny quad: (pos3 + uv2)
    m_QuadVA = Engine::VertexArray::Create();

    float verts[] = {
        // x,    y,    z,   u,   v
        -0.5f, -0.5f, 0.f, 0.f, 0.f,
         0.5f, -0.5f, 0.f, 1.f, 0.f,
         0.5f,  0.5f, 0.f, 1.f, 1.f,
        -0.5f,  0.5f, 0.f, 0.f, 1.f,
    };
    uint32_t idx[] = { 0,1,2, 2,3,0 };

    m_QuadVB = Engine::VertexBuffer::Create(verts, sizeof(verts));
    m_QuadVB->SetLayout({
        { Engine::ShaderDataType::Float3, "a_Position" },
        { Engine::ShaderDataType::Float2, "a_TexCoord" }
        });

    m_QuadIB = Engine::IndexBuffer::Create(idx, 6);

    m_QuadVA->AddVertexBuffer(m_QuadVB);
    m_QuadVA->SetIndexBuffer(m_QuadIB);

    // wartoœci sta³e shaderów
    m_RainbowShader->Bind();
    m_RainbowShader->SetFloat("u_Saturation", m_RainbowSaturation);
    m_RainbowShader->SetFloat("u_Value", m_RainbowValue);
    m_RainbowShader->SetFloat("u_Alpha", m_RainbowAlpha);
    m_RainbowShader->SetFloat("u_Speed", m_RainbowSpeed);
    m_RainbowShader->SetFloat("u_Tiling", m_RainbowTiling);
    m_RainbowShader->Unbinding();

    m_UI = std::make_unique<PongUI>(
        m_RainbowShader.get(),
        m_RainbowSpeed, m_RainbowTiling, m_RainbowSaturation, m_RainbowValue, m_RainbowAlpha,
        m_ScoreL, m_ScoreR,
        [this]() { ResetRound((rand() % 2) ? +1 : -1); }
    );

    // start rundy
    ResetRound(/*serveDir*/ (rand() % 2) ? +1 : -1);
}

void PongLayer::OnDetach()
{
    m_UI.reset();
    m_QuadIB.reset();
    m_QuadVB.reset();
    m_QuadVA.reset();
    m_RainbowShader.reset();
}

void PongLayer::OnUpdate(Engine::Timestep dt)
{
    m_Time += dt.GetSeconds();

    m_Paddles.Update(dt);
    UpdateBall(dt);

    // kolizje: odbicie obs³uguje Paddles
    if (m_Paddles.BounceBallIfNeeded(m_BallPos, m_BallVel, m_BallSize)) {
        // ewentualne globalne ograniczenia pi³ki po odbiciu
        m_BallVel.y = glm::clamp(m_BallVel.y, -18.0f, 18.0f);
    }

    // render
    Engine::RenderCommand::SetClearColor({ 0.05f, 0.07f, 0.1f, 1.0f });
    Engine::RenderCommand::Clear();

    Engine::Renderer2D::StartScene(*m_Camera);

    for (float y = -m_HalfWorldHeight + 0.5f; y <= m_HalfWorldHeight - 0.5f; y += 1.5f)
        Engine::Renderer2D::DrawQuad({ 0.0f, y, 0.0f }, { 0.2f, 0.8f }, { 1,1,1,0.25f });

    Engine::Renderer2D::DrawQuad({ m_BallPos.x, m_BallPos.y, 0.0f }, m_BallSize, { 1,1,1,1 });

    DrawPaddleRainbow(m_Paddles.GetLeftPos(), m_Paddles.GetSize());
    DrawPaddleRainbow(m_Paddles.GetRightPos(), m_Paddles.GetSize());

    Engine::Renderer2D::EndScene();
}

void PongLayer::OnImGuiRender()
{
    if (m_UI) m_UI->Draw("Pong");
}

void PongLayer::OnEvent(Engine::Event& e)
{
    Engine::EventDispatcher d(e);
    d.Dispatch<Engine::WindowResizeEvent>([this](auto& ev) { return HandleResize(ev); });
}

bool PongLayer::HandleResize(Engine::WindowResizeEvent& e)
{
    RecreateCamera(e.GetWidth(), e.GetHeight());
    return false;
}

void PongLayer::RecreateCamera(uint32_t width, uint32_t height)
{
    const float aspect = (height > 0) ? (float)width / (float)height : 16.0f / 9.0f;
    const float bottom = -m_HalfWorldHeight;
    const float top = +m_HalfWorldHeight;
    const float left = bottom * aspect;
    const float right = top * aspect;

    m_Camera = std::make_unique<Engine::OrthographicCamera>(left, right, bottom, top);
}

void PongLayer::ResetRound(int serveDir)
{
    m_BallPos = { 0.0f, 0.0f };
    float baseX = 9.0f + (rand() / (float)RAND_MAX) * 4.0f; // 9..13
    float baseY = 4.0f + (rand() / (float)RAND_MAX) * 6.0f; // 4..10
    m_BallVel = { serveDir * baseX, ((rand() % 2) ? +1.0f : -1.0f) * baseY };

    m_Paddles.ResetY();
}

void PongLayer::UpdateBall(Engine::Timestep dt)
{
    m_BallPos += m_BallVel * dt.GetSeconds();

    // góra/dó³
    const float topY = m_HalfWorldHeight - m_BallSize.y * 0.5f;
    const float bottomY = -m_HalfWorldHeight + m_BallSize.y * 0.5f;
    if (m_BallPos.y > topY) { m_BallPos.y = topY;    m_BallVel.y *= -1.0f; }
    if (m_BallPos.y < bottomY) { m_BallPos.y = bottomY; m_BallVel.y *= -1.0f; }

    // punkt dla lewej/prawej strony?
    const float outX = (m_HalfWorldHeight * (16.0f / 9.0f)) + 0.5f; // przybli¿ony „horyzont”
    if (m_BallPos.x < -outX) { ++m_ScoreR; ResetRound(+1); }
    if (m_BallPos.x > +outX) { ++m_ScoreL; ResetRound(-1); }
}

static inline bool AABB(const glm::vec2& p0, const glm::vec2& s0,
    const glm::vec2& p1, const glm::vec2& s1)
{
    const glm::vec2 h0 = s0 * 0.5f;
    const glm::vec2 h1 = s1 * 0.5f;
    const bool x = std::abs(p0.x - p1.x) <= (h0.x + h1.x);
    const bool y = std::abs(p0.y - p1.y) <= (h0.y + h1.y);
    return x && y;
}

void PongLayer::CheckCollisions()
{
    // Jeœli zostawiasz tê funkcjê, mo¿esz wywo³aæ tu jedynie:
    if (m_Paddles.BounceBallIfNeeded(m_BallPos, m_BallVel, m_BallSize)) {
        m_BallVel.y = glm::clamp(m_BallVel.y, -18.0f, 18.0f);
    }
}

void PongLayer::DrawPaddleRainbow(const glm::vec2& pos, const glm::vec2& size)
{
    if (!m_RainbowShader || !m_QuadVA) return;

    const glm::mat4 cameraViewProjection = m_Camera->ShareViewProjectionMatrix();

    glm::mat4 objectMatrix(1.0f);
    objectMatrix = glm::translate(objectMatrix, glm::vec3(pos, 0.0f));
    objectMatrix = glm::scale(objectMatrix, glm::vec3(size, 1.0f));

    m_RainbowShader->Bind();
    m_RainbowShader->SetMat4("u_ViewProjection", cameraViewProjection);
    m_RainbowShader->SetMat4("u_Transform", objectMatrix);
    m_RainbowShader->SetFloat("u_Time", m_Time);

    m_QuadVA->Bind();
    Engine::RenderCommand::DrawIndexed(m_QuadVA);
}