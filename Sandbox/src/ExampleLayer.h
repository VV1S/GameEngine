#pragma once

#include "Engine.h"

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Engine::Event& e) override;

private:
	Engine::ShaderLibrary m_ShaderLibrary;
	Engine::Shared<Engine::Shader> m_Shader;
	Engine::Shared<Engine::VertexArray> m_VertexArray;

	Engine::Shared<Engine::Shader> m_FlatColorShader;
	Engine::Shared<Engine::VertexArray> m_SquareVA;

	Engine::Shared<Engine::Texture2D> m_Texture;
	Engine::Shared<Engine::Texture2D> m_ChernoLogoTexture;

	Engine::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};
