#pragma once

#include "Engine.h"

class Sandbox2D : public Engine::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() override = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Engine::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Engine::Event& e) override;

private:
	Engine::OrthographicCameraController m_CameraController;

	// Temporary objects
	Engine::Shared<Engine::VertexArray> m_SquareVA;
	Engine::Shared<Engine::Shader> m_FlatColorShader;
	Engine::Shared<Engine::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.9f, 0.2f, 0.7f, 1.0f };
};
