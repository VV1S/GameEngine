#include <gtest/gtest.h>
#include <glad/glad.h>            
#include "Engine/Core/Application.h"
#include "Engine/Events/ApplicationEvent.h"

namespace Engine {
    struct ApplicationTestAccess {
        static void DispatchResize(Application& app, uint32_t w, uint32_t h) {
            WindowResizeEvent e(w, h);
            app.HandleEvent(e);
        }
    };
} // namespace Engine

TEST(Integration, GLViewportUpdatesOnResize)
{
    using namespace Engine;

    // Creates window + GL context + Renderer::Init()
    Application app;

    // sanity: we have GL at all
    ASSERT_NE(glGetString(GL_VENDOR), nullptr);
    ASSERT_NE(glGetString(GL_RENDERER), nullptr);

    // read current viewport
    GLint vp[4] = { 0,0,0,0 };
    glGetIntegerv(GL_VIEWPORT, vp);
    // e.g. typically 1280x720 – but do not assume "hardcoded"
    const GLint initialW = vp[2];
    const GLint initialH = vp[3];
    ASSERT_GT(initialW, 0);
    ASSERT_GT(initialH, 0);

    // send resize event and check if viewport adjusted
    const int newW = 640, newH = 360;
    ApplicationTestAccess::DispatchResize(app, newW, newH);

    GLint vp2[4] = { 0,0,0,0 };
    glGetIntegerv(GL_VIEWPORT, vp2);

    EXPECT_EQ(vp2[0], 0);
    EXPECT_EQ(vp2[1], 0);
    EXPECT_EQ(vp2[2], newW);
    EXPECT_EQ(vp2[3], newH);
}
