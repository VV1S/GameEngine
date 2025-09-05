#pragma once
#include "Engine/Core/Layer.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"


#include <memory>
struct ImFont;

class Course;


class PlaySceneLayer final : public Engine::Layer
{
public:
	PlaySceneLayer();
	~PlaySceneLayer() override;


	void OnAttach() override;
	void OnDetach() override;


	void OnUpdate(Engine::Timestep dt) override;
	void OnImGuiRender() override;
	void OnEvent(Engine::Event& e) override;


private:
	bool HandleResize(Engine::WindowResizeEvent& e);
	bool HandleMousePress(Engine::MouseButtonPressedEvent& e);


	void RecreateCamera(uint32_t w, uint32_t h);
	void DrawHUD();


private:
	enum class Mode { Menu, Running, Defeat };


	std::unique_ptr<Engine::OrthographicCamera> m_Camera;
	std::unique_ptr<Course> m_Course;


	Mode m_Mode = Mode::Menu;
	float m_Time = 0.0f;
	float m_BlinkPhase = 0.0f;

	// HUD fonts
	ImFont * m_FontTitle = nullptr; // np. 96 px
	ImFont * m_FontScore = nullptr; // np. 48 px
};