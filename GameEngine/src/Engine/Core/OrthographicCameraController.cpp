#include "enginepch.h"
#include "OrthographicCameraController.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h" // uses your EG_KEY_* constants
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace Engine {

    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool allowRotation)
        : m_Aspect(aspectRatio)
        , m_RotationEnabled(allowRotation)
        , m_Camera(-m_Aspect * m_Zoom, m_Aspect* m_Zoom, -m_Zoom, m_Zoom)
    {
        // Initial projection set from aspect & zoom
    }

    void OrthographicCameraController::OnUpdate(Timestep dt)
    {
        const float delta = dt;

        // Pan
        if (Input::IsKeyPressed(m_Controls.MoveLeft))  m_Position.x -= m_PanSpeed * delta;
        if (Input::IsKeyPressed(m_Controls.MoveRight)) m_Position.x += m_PanSpeed * delta;
        if (Input::IsKeyPressed(m_Controls.MoveUp))    m_Position.y += m_PanSpeed * delta;
        if (Input::IsKeyPressed(m_Controls.MoveDown))  m_Position.y -= m_PanSpeed * delta;

        // Rotation (optional)
        if (m_RotationEnabled)
        {
            if (Input::IsKeyPressed(m_Controls.RotCCW)) m_RotationDeg += m_RotateSpeedDeg * delta;
            if (Input::IsKeyPressed(m_Controls.RotCW))  m_RotationDeg -= m_RotateSpeedDeg * delta;

            // Wrap to [-180, 180] to avoid drift
            if (m_RotationDeg > 180.0f)  m_RotationDeg -= 360.0f;
            if (m_RotationDeg < -180.0f) m_RotationDeg += 360.0f;

            m_Camera.SetRotation(m_RotationDeg);
        }

        // Apply translation
        m_Camera.SetPosition(m_Position);

        // Optionally adapt pan speed to zoom (feels nicer when zoomed-in)
        m_PanSpeed = std::max(0.25f, m_Zoom);
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(
            BindEvent(this, &OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(
            BindEvent(this, &OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        SetZoomLevel(m_Zoom - e.GetYOffset() * m_ZoomStep);
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        const float w = static_cast<float>(e.GetWidth());
        const float h = static_cast<float>(e.GetHeight());
        if (w <= 0.0f || h <= 0.0f) return false;

        SetAspectRatio(w / h);
        return false;
    }

    void OrthographicCameraController::SetZoomLevel(float z)
    {
        m_Zoom = std::clamp(z, m_MinZoom, m_MaxZoom);
        RecalculateProjection();
    }

    void OrthographicCameraController::SetZoomLimits(float minZ, float maxZ)
    {
        if (minZ > maxZ) std::swap(minZ, maxZ);
        m_MinZoom = std::max(0.001f, minZ);
        m_MaxZoom = std::max(m_MinZoom, maxZ);
        SetZoomLevel(m_Zoom); // clamp and update
    }

    void OrthographicCameraController::SetAspectRatio(float aspect)
    {
        m_Aspect = std::max(0.001f, aspect);
        RecalculateProjection();
    }

    void OrthographicCameraController::Reset()
    {
        m_Position = { 0.0f, 0.0f, 0.0f };
        m_RotationDeg = 0.0f;
        m_Zoom = std::clamp(m_Zoom, m_MinZoom, m_MaxZoom);

        m_Camera.SetPosition(m_Position);
        m_Camera.SetRotation(m_RotationDeg);
        RecalculateProjection();
    }

    void OrthographicCameraController::RecalculateProjection()
    {
        m_Camera.SetProjection(-m_Aspect * m_Zoom, m_Aspect * m_Zoom, -m_Zoom, m_Zoom);
    }

} // namespace Engine
