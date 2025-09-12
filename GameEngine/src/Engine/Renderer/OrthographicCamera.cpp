#include "enginepch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

namespace Engine {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top,
        float zNear, float zFar)
    {
        EG_PROFILE_FUNCTION();
        m_Bounds = { left, right, bottom, top };
        m_Near = zNear;
        m_Far = zFar;
        m_ViewDirty = m_ProjDirty = m_VPDirty = true;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        EG_PROFILE_FUNCTION();
        m_Bounds = { left, right, bottom, top };
        MarkProjDirty();
    }

    void OrthographicCamera::SetProjection(const Bounds& b, float zNear, float zFar)
    {
        EG_PROFILE_FUNCTION();
        m_Bounds = b;
        m_Near = zNear;
        m_Far = zFar;
        MarkProjDirty();
    }

    void OrthographicCamera::SetPosition(const glm::vec3& position)
    {
        EG_PROFILE_FUNCTION();
        if (m_Position == position) return;
        m_Position = position;
        MarkViewDirty();
    }

    void OrthographicCamera::SetRotationRadians(float radians)
    {
        EG_PROFILE_FUNCTION();
        if (m_RotationRad == radians) return;
        m_RotationRad = radians;
        MarkViewDirty();
    }

    void OrthographicCamera::SetRotationDegrees(float degrees)
    {
        EG_PROFILE_FUNCTION();
        SetRotationRadians(glm::radians(degrees));
    }

    const glm::mat4& OrthographicCamera::GetProjectionMatrix() const
    {
        RebuildProjIfNeeded();
        return m_Projection;
    }

    const glm::mat4& OrthographicCamera::GetViewMatrix() const
    {
        RebuildViewIfNeeded();
        return m_View;
    }

    const glm::mat4& OrthographicCamera::ShareViewProjectionMatrix() const
    {
        RebuildViewProjIfNeeded();
        return m_ViewProjection;
    }

    void OrthographicCamera::MarkViewDirty()
    {
        m_ViewDirty = true;
        m_VPDirty = true;
    }

    void OrthographicCamera::MarkProjDirty()
    {
        m_ProjDirty = true;
        m_VPDirty = true;
    }

    void OrthographicCamera::RebuildViewIfNeeded() const
    {
        if (!m_ViewDirty) return;

        const glm::mat4 T = glm::translate(glm::mat4(1.0f), m_Position);
        const glm::mat4 R = glm::rotate(glm::mat4(1.0f), m_RotationRad, glm::vec3(0.0f, 0.0f, 1.0f));
        const glm::mat4 M = T * R;

        m_View = glm::inverse(M);

        m_ViewDirty = false;
        m_VPDirty = true;
    }

    void OrthographicCamera::RebuildProjIfNeeded() const
    {
        if (!m_ProjDirty) return;

        m_Projection = glm::ortho(m_Bounds.Left, m_Bounds.Right,
            m_Bounds.Bottom, m_Bounds.Top,
            m_Near, m_Far);

        m_ProjDirty = false;
        m_VPDirty = true;
    }

    void OrthographicCamera::RebuildViewProjIfNeeded() const
    {
        if (!m_VPDirty) return;
        RebuildProjIfNeeded();
        RebuildViewIfNeeded();
        m_ViewProjection = m_Projection * m_View;
        m_VPDirty = false;
    }

} // namespace Engine
