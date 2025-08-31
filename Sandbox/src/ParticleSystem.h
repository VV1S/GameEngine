#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Engine/Core/Timestep.h"
#include "Engine/Renderer/Renderer2D.h"

struct ParticleProps
{
    glm::vec2 Position{ 0.0f };
    glm::vec2 Velocity{ 0.0f };
    glm::vec2 VelocityVariation{ 0.0f };

    glm::vec4 ColorBegin{ 1.0f };
    glm::vec4 ColorEnd{ 0.0f, 0.0f, 0.0f, 0.0f };

    float SizeBegin = 1.0f;
    float SizeEnd = 0.0f;
    float SizeVariation = 0.0f;

    float LifeTime = 1.0f;
};

class ParticleSystem
{
public:
    ParticleSystem();

    void Emit(const ParticleProps& props);
    void OnUpdate(Engine::Timestep ts);
    void OnRender();

    void SetPoolSize(size_t count);

private:
    struct Particle
    {
        bool Active = false;

        glm::vec2 Position{ 0.0f };
        glm::vec2 Velocity{ 0.0f };
        float Rotation = 0.0f;

        glm::vec4 ColorBegin{ 1.0f };
        glm::vec4 ColorEnd{ 0.0f };

        float SizeBegin = 1.0f;
        float SizeEnd = 0.0f;

        float LifeTime = 1.0f;
        float LifeRemaining = 0.0f;
    };

    std::vector<Particle> m_Pool;
    size_t m_PoolIndex = 0; // next slot to overwrite (ring buffer)
};
