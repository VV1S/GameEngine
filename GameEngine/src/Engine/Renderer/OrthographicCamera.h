#pragma once
#include <glm/glm.hpp>

namespace Engine {

    class OrthographicCamera
    {
    public:
        struct Bounds {
            float Left = -1.0f;
            float Right = 1.0f;
            float Bottom = -1.0f;
            float Top = 1.0f;
        };

        OrthographicCamera(float left, float right, float bottom, float top,
            float zNear = -1.0f, float zFar = 1.0f);

        void SetProjection(float left, float right, float bottom, float top);
        void SetProjection(const Bounds& b, float zNear, float zFar);

        const glm::vec3& GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec3& position);

        float GetRotationRadians() const { return m_RotationRad; }
        void  SetRotationRadians(float radians);
        void  SetRotationDegrees(float degrees);

        // Backward-compat: degrees API
        float GetRotation() const { return glm::degrees(m_RotationRad); }
        void  SetRotation(float degrees) { SetRotationDegrees(degrees); }

        // Mark const — bêd¹ przebudowywa³y cache (mutable)
        const glm::mat4& GetProjectionMatrix() const;
        const glm::mat4& GetViewMatrix() const;
        const glm::mat4& GetViewProjectionMatrix() const;

        const Bounds& GetBounds() const { return m_Bounds; }
        float GetNear() const { return m_Near; }
        float GetFar()  const { return m_Far; }

    private:
        void MarkViewDirty();
        void MarkProjDirty();


        void RebuildViewIfNeeded() const;
        void RebuildProjIfNeeded() const;
        void RebuildViewProjIfNeeded() const;

    private:
        // Transform
        glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
        float     m_RotationRad = 0.0f; // Z-rot radians

        Bounds m_Bounds{};
        float  m_Near = -1.0f;
        float  m_Far = 1.0f;

        // Cache (mutable because rebuild in const getters)
        mutable glm::mat4 m_Projection{ 1.0f };
        mutable glm::mat4 m_View{ 1.0f };
        mutable glm::mat4 m_ViewProjection{ 1.0f };

        mutable bool m_ViewDirty = true;
        mutable bool m_ProjDirty = true;
        mutable bool m_VPDirty = true;
    };

} // namespace Engine
