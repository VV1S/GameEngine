#pragma once
#include <glm/glm.hpp>
#include "Engine/Core/Timestep.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Core/Input.h"
#include "Engine/Renderer/Texture.h"
#include "FXSystem.h"


class Ship {
public:
	Ship();


	void LoadAssets();
	void Update(Engine::Timestep dt);
	void Render() const;
	void Ui();


	void Reset();


	float HeadingDeg() const { return m_Vel.y * 4.0f - 90.0f; }
	const glm::vec2& Position() const { return m_Pos; }


	uint32_t Score() const { return (uint32_t)((m_Pos.x + 10.0f) / 10.0f); }


private:
	glm::vec2 m_Pos{ -10.0f, 0.0f };
	glm::vec2 m_Vel{ 5.0f, 0.0f };


	float m_Thrust = 0.5f;
	float m_Gravity = 0.4f;


	float m_Time = 0.0f, m_SmokeT = 0.0f, m_SmokeEvery = 0.4f;


	FXSystem m_FX;
	FXSpec m_Smoke, m_Flame;


	Engine::Shared<Engine::Texture2D> m_Tex;
};