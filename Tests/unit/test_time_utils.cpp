#include <gtest/gtest.h>
#include <cmath>
#include <Core/Timestep.h>
// #include <Engine/Core/Timestep.h>

TEST(Time, TimestepMath) {
    // estimated interval 16.666 ms -> 60 FPS
    const float dt = 1.0f / 60.0f;
     Engine::Timestep ts(dt);
     EXPECT_NEAR((float)ts, dt, 1e-6f);
}
