#include "enginepch.h"
#include "ParticleSystem.h"

#include "Random.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>  // glm::lerp
#include <glm/gtc/constants.hpp>      // glm::pi

ParticleSystem::ParticleSystem()
{
    SetPoolSize(1000);
}

void ParticleSystem::SetPoolSize(size_t count)
{
    m_Pool.clear();
    m_Pool.resize(count);
    m_PoolIndex = 0;
}

void ParticleSystem::Emit(const ParticleProps& props)
{
    Particle& p = m_Pool[m_PoolIndex];
    p.Active = true;

    p.Position = props.Position;
    p.Rotation = Random::Float() * 2.0f * glm::pi<float>();

    p.Velocity = props.Velocity;
    p.Velocity.x += props.VelocityVariation.x * (Random::Float() - 0.5f);
    p.Velocity.y += props.VelocityVariation.y * (Random::Float() - 0.5f);

    p.ColorBegin = props.ColorBegin;
    p.ColorEnd = props.ColorEnd;

    p.SizeBegin = props.SizeBegin + props.SizeVariation * (Random::Float() - 0.5f);
    p.SizeEnd = props.SizeEnd;

    p.LifeTime = props.LifeTime;
    p.LifeRemaining = props.LifeTime;

    // advance ring buffer index
    m_PoolIndex = (m_PoolIndex + 1) % m_Pool.size();
}

void ParticleSystem::OnUpdate(Engine::Timestep ts)
{
    const float dt = ts.GetSeconds();

    for (auto& p : m_Pool)
    {
        if (!p.Active)
            continue;

        if (p.LifeRemaining <= 0.0f)
        {
            p.Active = false;
            continue;
        }

        p.LifeRemaining -= dt;
        p.Position += p.Velocity * dt;
        p.Rotation += 0.01f * dt;
    }
}

void ParticleSystem::OnRender()
{
    for (const auto& p : m_Pool)
    {
        if (!p.Active)
            continue;

        const float life = p.LifeRemaining / p.LifeTime;

        glm::vec4 color = glm::lerp(p.ColorEnd, p.ColorBegin, life);
        color.a *= life;

        const float size = glm::lerp(p.SizeEnd, p.SizeBegin, life);

        Engine::Renderer2D::DrawRotatedQuad(p.Position, { size, size }, p.Rotation, color);
    }
}
