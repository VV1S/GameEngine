#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Engine/Core/Timestep.h"
#include "Engine/Renderer/Renderer2D.h"


struct FXSpec {
	glm::vec2 pos{ 0 };
	glm::vec2 vel{ 0 };
	glm::vec2 velJitter{ 0 };


	glm::vec4 colorStart{ 1 };
	glm::vec4 colorEnd{ 0,0,0,0 };


	float sizeStart = 1.0f;
	float sizeEnd = 0.0f;
	float sizeJitter = 0.0f;


	float life = 1.0f;
};


class FXSystem {
public:
	FXSystem();
	void ResizePool(size_t n);


	void Spawn(const FXSpec& s);
	void Update(Engine::Timestep dt);
	void Render() const;


private:
	struct Node {
		bool live = false;
		glm::vec2 pos{ 0 }, vel{ 0 };
		float rot = 0.0f;


		glm::vec4 c0{ 1 }, c1{ 0 };
		float s0 = 1.0f, s1 = 0.0f;
		float life = 1.0f, left = 0.0f;
	};


	std::vector<Node> m_Pool;
	size_t m_Head = 0; // ring buffer
};