#include <gtest/gtest.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Core/OrthographicCameraController.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "helpers/InputTestHelpers.h"

using namespace Engine;
using TestHelpers::FakeInputBackend;

namespace {
    inline bool MatNear(const glm::mat4& a, const glm::mat4& b, float eps) {
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                if (std::abs(a[r][c] - b[r][c]) > eps) return false;
        return true;
    }
}

// --------- OrthographicCamera ---------
TEST(OrthographicCamera_Basics, ProjectionAndViewChange)
{
    OrthographicCamera cam(-2.f, 2.f, -1.f, 1.f, -1.f, 1.f);

    auto expected = glm::ortho(-2.f, 2.f, -1.f, 1.f, -1.f, 1.f);
    EXPECT_TRUE(MatNear(cam.GetProjectionMatrix(), expected, 1e-4f));

    cam.SetProjection(-4.f, 4.f, -3.f, 3.f);
    auto expected2 = glm::ortho(-4.f, 4.f, -3.f, 3.f, -1.f, 1.f);
    EXPECT_TRUE(MatNear(cam.GetProjectionMatrix(), expected2, 1e-4f));

    cam.SetPosition({ 1.f, 2.f, 0.f });
    cam.SetRotationDegrees(90.f);
    const glm::mat4 view1 = cam.GetViewMatrix();
    const glm::mat4 view2 = cam.GetViewMatrix();
    EXPECT_TRUE(MatNear(view1, view2, 1e-4f));
}

TEST(OrthographicCamera_Basics, ViewProjectionIsConsistent)
{
    OrthographicCamera cam(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
    cam.SetPosition({ 0.5f, -0.25f, 0.f });
    cam.SetRotationDegrees(30.f);

    const glm::mat4 vp1 = cam.GetViewProjectionMatrix();
    const glm::mat4 vp2 = cam.GetViewProjectionMatrix();
    EXPECT_TRUE(MatNear(vp1, vp2, 1e-4f));
}

class CameraControllerFixture : public ::testing::Test {
protected:
    void SetUp() override {
        m_fake = new FakeInputBackend();
        Input::Initialize(Engine::Unique<InputBackend>(m_fake)); // ownership -> Input
    }
    void TearDown() override {
        Input::Shutdown();
        m_fake = nullptr;
    }
    FakeInputBackend* fake() { return m_fake; }
private:
    FakeInputBackend* m_fake = nullptr;
};

// --------- OrthographicCameraController ---------
TEST_F(CameraControllerFixture, PanMovesCameraWithKeys)
{
    OrthographicCameraController ctrl(16.f / 9.f, false);

    fake()->press(EG_KEY_A);
    fake()->press(EG_KEY_W);

    ctrl.OnUpdate(Timestep(0.5f)); // 0.5 s

    const glm::vec3 pos = ctrl.GetCamera().GetPosition();
    EXPECT_NEAR(pos.x, -2.5f, 1e-4f);
    EXPECT_NEAR(pos.y, 2.5f, 1e-4f);
    EXPECT_NEAR(pos.z, 0.0f, 1e-4f);

    fake()->clear();
}

TEST_F(CameraControllerFixture, RotationEnabledAndWrapsToMinus180To180)
{
    OrthographicCameraController ctrl(1.0f, true);
    ctrl.SetRotateSpeed(100.f);

    fake()->press(EG_KEY_Q);
    ctrl.OnUpdate(Timestep(2.0f));
    float rotDeg = ctrl.GetCamera().GetRotation();
    EXPECT_NEAR(rotDeg, -160.0f, 1e-4f);

    fake()->clear();

    fake()->press(EG_KEY_E);
    ctrl.OnUpdate(Timestep(0.3f));
    rotDeg = ctrl.GetCamera().GetRotation();
    EXPECT_NEAR(rotDeg, 170.0f, 1e-4f);

    fake()->clear();
}

TEST_F(CameraControllerFixture, MouseScrollZoomIsClampedAndUpdatesProjection)
{
    OrthographicCameraController ctrl(2.0f, false);
    ctrl.SetZoomLimits(0.5f, 2.0f);
    ctrl.SetZoomStep(0.5f);

    {
        const auto& b = ctrl.GetCamera().GetBounds();
        EXPECT_NEAR(b.Left, -2.0f, 1e-4f);
        EXPECT_NEAR(b.Right, 2.0f, 1e-4f);
        EXPECT_NEAR(b.Bottom, -1.0f, 1e-4f);
        EXPECT_NEAR(b.Top, 1.0f, 1e-4f);
    }

    MouseScrolledEvent scroll(0.f, +2.f);
    ctrl.OnEvent(scroll);
    {
        const auto& b = ctrl.GetCamera().GetBounds();
        EXPECT_NEAR(b.Left, -1.0f, 1e-4f);
        EXPECT_NEAR(b.Right, 1.0f, 1e-4f);
        EXPECT_NEAR(b.Bottom, -0.5f, 1e-4f);
        EXPECT_NEAR(b.Top, 0.5f, 1e-4f);
    }

    MouseScrolledEvent scroll2(0.f, -6.f);
    ctrl.OnEvent(scroll2);
    {
        const auto& b = ctrl.GetCamera().GetBounds();
        EXPECT_NEAR(b.Left, -4.0f, 1e-4f);
        EXPECT_NEAR(b.Right, 4.0f, 1e-4f);
        EXPECT_NEAR(b.Bottom, -2.0f, 1e-4f);
        EXPECT_NEAR(b.Top, 2.0f, 1e-4f);
    }
}

TEST_F(CameraControllerFixture, WindowResizeUpdatesAspectAndProjection)
{
    OrthographicCameraController ctrl(2.0f, false);

    WindowResizeEvent resize(400, 800); // aspect 0.5
    ctrl.OnEvent(resize);

    EXPECT_NEAR(ctrl.GetAspectRatio(), 0.5f, 1e-4f);
    const auto& b = ctrl.GetCamera().GetBounds();
    EXPECT_NEAR(b.Left, -0.5f, 1e-4f);
    EXPECT_NEAR(b.Right, 0.5f, 1e-4f);
    EXPECT_NEAR(b.Bottom, -1.0f, 1e-4f);
    EXPECT_NEAR(b.Top, 1.0f, 1e-4f);
}

TEST_F(CameraControllerFixture, ResetRestoresPoseAndKeepsClampedZoom)
{
    OrthographicCameraController ctrl(1.5f, true);

    fake()->press(EG_KEY_D);
    ctrl.OnUpdate(Timestep(1.0f));
    fake()->clear();

    ctrl.SetZoomLimits(0.25f, 1.0f);
    ctrl.SetZoomLevel(5.0f); // clamp -> 1.0
    ctrl.GetCamera().SetRotation(45.f);

    ctrl.Reset();

    const auto pos = ctrl.GetCamera().GetPosition();
    EXPECT_NEAR(pos.x, 0.0f, 1e-4f);
    EXPECT_NEAR(pos.y, 0.0f, 1e-4f);
    EXPECT_NEAR(pos.z, 0.0f, 1e-4f);

    EXPECT_NEAR(ctrl.GetCamera().GetRotation(), 0.0f, 1e-4f);

    const auto& b = ctrl.GetCamera().GetBounds(); // aspect 1.5 * zoom 1.0
    EXPECT_NEAR(b.Left, -1.5f, 1e-4f);
    EXPECT_NEAR(b.Right, 1.5f, 1e-4f);
    EXPECT_NEAR(b.Bottom, -1.0f, 1e-4f);
    EXPECT_NEAR(b.Top, 1.0f, 1e-4f);
}
