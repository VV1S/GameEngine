#pragma once
#include <memory>
#include <glm/glm.hpp>

#include "Engine/Core/Layer.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "PongUI.h"
#include "Paddles.h"

class PongLayer final : public Engine::Layer
{
public:
    PongLayer();
    ~PongLayer() override;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(Engine::Timestep dt) override;
    void OnImGuiRender() override;
    void OnEvent(Engine::Event& e) override;

private:
    bool HandleResize(Engine::WindowResizeEvent& e);
    void RecreateCamera(uint32_t w, uint32_t h);

    // logika gry
    void ResetRound(int serveDir); // -1 serwuje w lewo, +1 w prawo
    void UpdateBall(Engine::Timestep dt);
    void CheckCollisions();

    // rysowanie paletki z shaderem
    void DrawPaddleRainbow(const glm::vec2& pos, const glm::vec2& size);

private:
    // kamera ortograficzna
    std::unique_ptr<Engine::OrthographicCamera> m_Camera;

    // parametry œwiata (proporcje jak w Twoich scenach)
    float m_HalfWorldHeight = 8.0f;

    // kontroler paletek (pozycje, sterowanie, clamp)
    Paddles m_Paddles{ {0.6f, 2.8f}, 14.0f, /*halfWorldHeight*/8.0f, 0.2f, -12.0f, +12.0f };

    // pi³ka
    glm::vec2 m_BallPos{ 0.0f, 0.0f };
    glm::vec2 m_BallVel{ 9.5f, 7.0f };
    glm::vec2 m_BallSize{ 0.6f, 0.6f };

    int m_ScoreL = 0;
    int m_ScoreR = 0;

    // czas (do animacji shadera)
    float m_Time = 0.0f;

    // zasoby do rysowania paletek shaderem
    Engine::Shared<Engine::Shader>       m_RainbowShader;
    Engine::Shared<Engine::VertexArray>  m_QuadVA;
    Engine::Shared<Engine::VertexBuffer> m_QuadVB;
    Engine::Shared<Engine::IndexBuffer>  m_QuadIB;

    std::unique_ptr<PongUI> m_UI;

    // uniformy shadera (edytowalne w ImGui)
    float m_RainbowSpeed = 0.35f;
    float m_RainbowTiling = 1.75f;
    float m_RainbowSaturation = 1.0f;
    float m_RainbowValue = 1.0f;
    float m_RainbowAlpha = 1.0f;
};
