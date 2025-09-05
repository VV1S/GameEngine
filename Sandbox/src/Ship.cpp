#include "enginepch.h"
#include "Ship.h"
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Engine.h>


Ship::Ship()
{
	// smoke
	m_Smoke.vel = { -2.0f, 0.0f }; m_Smoke.velJitter = { 4.0f, 2.0f };
	m_Smoke.sizeStart = 0.35f; m_Smoke.sizeEnd = 0.0f; m_Smoke.sizeJitter = 0.15f;
	m_Smoke.colorStart = { 0.8f,0.8f,0.8f,1.0f };
	m_Smoke.colorEnd = { 0.6f,0.6f,0.6f,1.0f };
	m_Smoke.life = 4.0f;


	// fire
	m_Flame.vel = { -2.0f, 0.0f }; m_Flame.velJitter = { 3.0f, 1.0f };
	m_Flame.sizeStart = 0.5f; m_Flame.sizeEnd = 0.0f; m_Flame.sizeJitter = 0.3f;
	m_Flame.colorStart = { 254 / 255.f,109 / 255.f, 41 / 255.f, 1.0f };
	m_Flame.colorEnd = { 254 / 255.f,212 / 255.f,123 / 255.f, 1.0f };
	m_Flame.life = 1.0f;
}


void Ship::LoadAssets()
{
	m_Tex = Engine::Texture2D::Create("assets/textures/Ship.png");
}


void Ship::Update(Engine::Timestep dt)
{
	m_Time += dt;
	if (Engine::Input::IsKeyPressed(EG_KEY_SPACE))
	{
		m_Vel.y += m_Thrust;
		if (m_Vel.y < 0.0f) m_Vel.y += m_Thrust * 2.0f;
// fire emitter
glm::vec2 tail = { 0.0f, -0.6f };
float r = glm::radians(HeadingDeg());
glm::vec4 t = glm::rotate(glm::mat4(1.0f), r, { 0,0,1 }) * glm::vec4(tail, 0, 1);


m_Flame.pos = m_Pos + glm::vec2{ t.x, t.y };
m_Flame.vel.y = -m_Vel.y * 0.2f - 0.2f;
m_FX.Spawn(m_Flame);
}
else
{
	m_Vel.y -= m_Gravity;
}


m_Vel.y = glm::clamp(m_Vel.y, -20.0f, 20.0f);
m_Pos += m_Vel * (float)dt;


// smoke trail
if (m_Time > m_SmokeT) {
	m_Smoke.pos = m_Pos;
	m_FX.Spawn(m_Smoke);
	m_SmokeT += m_SmokeEvery;
}


m_FX.Update(dt);
}


void Ship::Render() const
{
	m_FX.Render();
	Engine::Renderer2D::DrawRotatedQuad(
		{ m_Pos.x, m_Pos.y, 0.5f },
		{ 1.0f, 1.3f },
		glm::radians(HeadingDeg()),
		m_Tex, 1.0f, glm::vec4(1.0f));
}


void Ship::Ui()
{
	ImGui::DragFloat("Thrust", &m_Thrust, 0.1f);
	ImGui::DragFloat("Gravity", &m_Gravity, 0.1f);
}


void Ship::Reset()
{
	m_Pos = { -10.0f, 0.0f };
	m_Vel = { 5.0f, 0.0f };
	m_Time = 0.0f; m_SmokeT = m_SmokeEvery;
}