#include "enginepch.h"
#include "FXSystem.h"
#include <glm/gtc/constants.hpp> 
#include <glm/common.hpp> 
#include <cstdlib>

namespace Engine {
	FXSystem::FXSystem() { ResizePool(1000); }


	void FXSystem::ResizePool(size_t n) {
		m_Pool.clear();
		m_Pool.resize(n);
		m_Head = 0;
	}


	void FXSystem::Spawn(const FXSpec& s)
	{
		Node& n = m_Pool[m_Head];
		n.live = true;
		n.pos = s.pos;
		n.vel = s.vel;
		n.vel.x += s.velJitter.x * ((rand() / (float)RAND_MAX) - 0.5f);
		n.vel.y += s.velJitter.y * ((rand() / (float)RAND_MAX) - 0.5f);
		n.rot = ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * glm::pi<float>();


		n.c0 = s.colorStart;
		n.c1 = s.colorEnd;


		n.s0 = s.sizeStart + s.sizeJitter * ((rand() / (float)RAND_MAX) - 0.5f);
		n.s1 = s.sizeEnd;


		n.life = s.life;
		n.left = s.life;


		m_Head = (m_Head + 1) % m_Pool.size();
	}


	void FXSystem::Update(Timestep dt)
	{
		const float d = dt.GetSeconds();
		for (auto& it : m_Pool) {
			if (!it.live) continue;
			it.left -= d;
			if (it.left <= 0.0f) { it.live = false; continue; }
			it.pos += it.vel * d;
			it.rot += 0.01f * d;
		}
	}


	void FXSystem::Render() const
	{
		for (const auto& it : m_Pool) {
			if (!it.live) continue;
			float t = it.left / it.life;
			glm::vec4 col = glm::mix(it.c1, it.c0, t); col.a *= t;
			float sz = glm::mix(it.s1, it.s0, t);
			Renderer2D::DrawRotatedQuad(it.pos, { sz, sz }, it.rot, col);
		}
	}
} // namespace Engine