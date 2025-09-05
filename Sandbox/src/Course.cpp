#include "enginepch.h"
#include "Course.h"
#include "RNG.h"


#include <glm/gtc/matrix_transform.hpp>
#include <cmath>


static glm::vec4 HSV2RGB(const glm::vec3& hsv) {
	int H = (int)(hsv.x * 360.0f);
	double S = hsv.y, V = hsv.z;
	double C = S * V;
	double X = C * (1 - std::fabs(std::fmod(H / 60.0, 2) - 1));
	double m = V - C;
	double R = 0, G = 0, B = 0;
	if (H < 60) { R = C; G = X; }
	else if (H < 120) { R = X; G = C; }
	else if (H < 180) { G = C; B = X; }
	else if (H < 240) { G = X; B = C; }
	else if (H < 300) { R = X; B = C; }
	else { R = C; B = X; }
	return { float(R + m), float(G + m), float(B + m), 1.0f };
}


static bool PointInTri(const glm::vec2& p, glm::vec2 a, const glm::vec2& b, const glm::vec2& c)
{
	auto s = a.y * c.x - a.x * c.y + (c.y - a.y) * p.x + (a.x - c.x) * p.y;
	auto t = a.x * b.y - a.y * b.x + (a.y - b.y) * p.x + (b.x - a.x) * p.y;
	if ((s < 0) != (t < 0)) return false;
	float A = -b.y * c.x + a.y * (c.x - b.x) + a.x * (b.y - c.y) + b.x * c.y;
	return A < 0 ? (s <= 0 && s + t >= A) : (s >= 0 && s + t <= A);
}


void Course::Initialize()
{
	Util::RNG::Init();
	m_TriTex = Engine::Texture2D::Create("assets/textures/Triangle.png");
	m_Ship.LoadAssets();


	m_Gates.resize(5);
	for (int i = 0; i < 5; ++i)
		SpawnGate(i, i * 10.0f);
}


void Course::Tick(Engine::Timestep dt)
{
	m_Ship.Update(dt);


	if (CheckCollision()) {
		m_Ended = true;
		return;
	}


	m_ColorHSV.x += 0.1f * dt;
	if (m_ColorHSV.x > 1.0f) m_ColorHSV.x = 0.0f;


	if (m_Ship.Position().x > m_NextGateAt) {
		SpawnGate(m_GateIdx, m_NextGateAt + 20.0f);
		m_GateIdx = (m_GateIdx + 1) % (int)m_Gates.size();
		m_NextGateAt += 10.0f;
	}
}


void Course::Draw()
{
	const auto& shipPos = m_Ship.Position();
	glm::vec4 col = HSV2RGB(m_ColorHSV);


	Engine::Renderer2D::DrawQuad({ shipPos.x, 0.0f, -0.8f }, { 50.0f, 50.0f }, { 0.3f,0.3f,0.3f,1 });
	Engine::Renderer2D::DrawQuad({ shipPos.x, 34.0f }, { 50.0f, 50.0f }, col);
	Engine::Renderer2D::DrawQuad({ shipPos.x,-34.0f }, { 50.0f, 50.0f }, col);


	for (auto& g : m_Gates) {
		Engine::Renderer2D::DrawRotatedQuad(g.topPos, g.topSize, glm::radians(180.0f), m_TriTex, 1.0f, col);
		Engine::Renderer2D::DrawRotatedQuad(g.botPos, g.botSize, 0.0f, m_TriTex, 1.0f, col);
	}


	m_Ship.Render();
}
void Course::Ui()
{
	m_Ship.Ui();
}


void Course::SpawnGate(int index, float x)
{
	Gate& g = m_Gates[index];
	g.topPos.x = x; g.botPos.x = x;
	g.topPos.z = index * 0.1f - 0.5f;
	g.botPos.z = index * 0.1f - 0.5f + 0.05f;


	float center = Util::RNG::Float01() * 35.0f - 17.5f;
	float gap = 2.0f + Util::RNG::Float01() * 5.0f;


	g.topPos.y = 10.0f - ((10.0f - center) * 0.2f) + gap * 0.5f;
	g.botPos.y = -10.0f - ((-10.0f - center) * 0.2f) - gap * 0.5f;
}


bool Course::CheckCollision() const
{
	if (glm::abs(m_Ship.Position().y) > 8.5f) return true;


	glm::vec4 verts[4] = { {-0.5f,-0.5f,0,1}, {0.5f,-0.5f,0,1}, {0.5f,0.5f,0,1}, {-0.5f,0.5f,0,1} };
	glm::vec4 out[4];
	auto pos = m_Ship.Position();
	auto rot = glm::radians(m_Ship.HeadingDeg());
	auto M = glm::translate(glm::mat4(1), { pos.x,pos.y,0 })
		* glm::rotate(glm::mat4(1), rot, { 0,0,1 })
		* glm::scale(glm::mat4(1), { 1.0f,1.3f,1.0f });
	for (int i = 0; i < 4; ++i) out[i] = M * verts[i];


	glm::vec4 tri[3] = {
	{-0.5f + 0.1f,-0.5f + 0.1f,0,1},
	{ 0.5f - 0.1f,-0.5f + 0.1f,0,1},
	{ 0.0f, 0.5f - 0.1f,0,1},
	};


	for (auto& g : m_Gates) {
		// TOP
		glm::vec2 T[3];
		auto Mt = glm::translate(glm::mat4(1), { g.topPos.x,g.topPos.y,0 })
			* glm::rotate(glm::mat4(1), glm::radians(180.0f), { 0,0,1 })
			* glm::scale(glm::mat4(1), { g.topSize.x,g.topSize.y,1 });
		for (int i = 0; i < 3; ++i) { auto v = Mt * tri[i]; T[i] = { v.x,v.y }; }
		for (auto& v : out) if (PointInTri({ v.x,v.y }, T[0], T[1], T[2])) return true;


		// BOTTOM
		glm::vec2 B[3];
		auto Mb = glm::translate(glm::mat4(1), { g.botPos.x,g.botPos.y,0 })
			* glm::scale(glm::mat4(1), { g.botSize.x,g.botSize.y,1 });
		for (int i = 0; i < 3; ++i) { auto v = Mb * tri[i]; B[i] = { v.x,v.y }; }
		for (auto& v : out) if (PointInTri({ v.x,v.y }, B[0], B[1], B[2])) return true;
	}
	return false;
}


void Course::Reset()
{
	m_Ended = false;
	m_Ship.Reset();
	m_NextGateAt = 30.0f;
	m_GateIdx = 0;
	for (int i = 0; i < 5; ++i) SpawnGate(i, i * 10.0f);
}