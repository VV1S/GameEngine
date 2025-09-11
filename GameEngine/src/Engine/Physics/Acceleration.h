#pragma once
#include <glm/glm.hpp>
#include "Engine/Core/Timestep.h"

namespace Engine
{
    class Acceleration {
    public:
        explicit Acceleration(glm::vec2 accel = { 0.0f, 0.0f }) : m_Accel(accel) {}

        // Direct set / get
        void        Set(glm::vec2 a) { m_Accel = a; }
        glm::vec2   Get() const { return m_Accel; }
        

        // Set using polar coordinates (in radians): |a| and angle relative to X axis (CCW)
        void SetFromPolar(float magnitude, float radians) {
            m_Accel.x = magnitude * std::cos(radians);
            m_Accel.y = magnitude * std::sin(radians);
        }
        float Magnitude() const { return glm::length(m_Accel); }
        float Angle()     const { return std::atan2(m_Accel.y, m_Accel.x); } // rad

        // Apply acceleration: v += a * dt
        void Apply(glm::vec2& velocity, Engine::Timestep dt) const {
            velocity += m_Accel * static_cast<float>(dt);
        }

    private:
        glm::vec2 m_Accel; // [unit./s^2]
    };
} // namespace Engine
