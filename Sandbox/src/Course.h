#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Engine/Core/Timestep.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Texture.h"
#include "Ship.h"


struct Gate {
	glm::vec3 topPos{ 0.0f, 10.0f, 0.0f };
	glm::vec2 topSize{ 15.0f, 20.0f };


	glm::vec3 botPos{ 10.0f, 10.0f, 0.0f };
	glm::vec2 botSize{ 15.0f, 20.0f };
};


class Course {
public:
	void Initialize();
	void Tick(Engine::Timestep dt);
	void Draw();


	bool Ended() const { return m_Ended; }
	void Reset();


	Ship& PlayerRef() { return m_Ship; }


private:
	void SpawnGate(int idx, float xOffset);
	bool CheckCollision() const;


private:
	Ship m_Ship;
	bool m_Ended = false;


	float m_NextGateAt = 30.0f;
	int m_GateIdx = 0;
	glm::vec3 m_ColorHSV{ 0.0f, 0.8f, 0.8f };


	std::vector<Gate> m_Gates;
	Engine::Shared<Engine::Texture2D> m_TriTex;
};