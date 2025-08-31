#pragma once

#include <algorithm>
#include <glm/glm.hpp>

#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Core/Timestep.h"

#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Core/KeyCodes.h" // use your key defines (EG_KEY_*)

namespace Engine {

    // Rebindable key mapping; defaults use your EG_KEY_* codes
    struct Camera2DControls
    {
        int MoveLeft = EG_KEY_A;
        int MoveRight = EG_KEY_D;
        int MoveUp = EG_KEY_W;
        int MoveDown = EG_KEY_S;
        int RotCCW = EG_KEY_Q;
        int RotCW = EG_KEY_E;
    };

    class OrthographicCameraController
    {
    public:
        explicit OrthographicCameraController(float aspectRatio, bool allowRotation = false);

        void OnUpdate(Timestep dt);
        void OnEvent(Event& e);

        // Camera access
        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }

        // Zoom / limits
        float GetZoomLevel() const { return m_Zoom; }
        void  SetZoomLevel(float z);                  // clamped to [m_MinZoom, m_MaxZoom]
        void  SetZoomLimits(float minZ, float maxZ);  // enforce sane bounds

        // Aspect
        void  SetAspectRatio(float aspect);
        float GetAspectRatio() const { return m_Aspect; }

        // Runtime tuning
        void SetRotationEnabled(bool enabled) { m_RotationEnabled = enabled; }
        bool IsRotationEnabled() const { return m_RotationEnabled; }

        void SetPanSpeed(float unitsPerSecond) { m_PanSpeed = unitsPerSecond; }
        void SetRotateSpeed(float degPerSecond) { m_RotateSpeedDeg = degPerSecond; }
        void SetZoomStep(float stepPerScrollTick) { m_ZoomStep = stepPerScrollTick; }
        void SetControls(const Camera2DControls& c) { m_Controls = c; }

        // Reset pose (position/orientation) while keeping aspect & clamped zoom
        void Reset();

    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
        void RecalculateProjection();

    private:
        // Projection params
        float m_Aspect = 1.0f;
        float m_Zoom = 1.0f;
        float m_MinZoom = 0.25f;
        float m_MaxZoom = 10.0f;

        // Pose
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        float     m_RotationDeg = 0.0f;

        // Behavior / tuning
        bool  m_RotationEnabled = false;
        float m_PanSpeed = 5.0f;    // world units / s
        float m_RotateSpeedDeg = 180.0f;  // deg / s
        float m_ZoomStep = 0.25f;   // per scroll tick

        Camera2DControls m_Controls{};

        // Camera
        OrthographicCamera m_Camera;
    };

} // namespace Engine
