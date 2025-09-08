#include <gtest/gtest.h>

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseButtonCodes.h"

using namespace Engine;

// --- Simple mock backend: configurable responses + counters ---
class FakeInputBackend : public InputBackend {
public:
    bool keyDown = false;
    bool mouseDown = false;
    std::pair<float, float> pos{ 0.f, 0.f };

    mutable int callsKey = 0;
    mutable int callsMouseButton = 0;
    mutable int callsPos = 0;
    mutable int callsX = 0;
    mutable int callsY = 0;

    bool IsKeyPressed(int /*keycode*/) const override {
        ++callsKey; return keyDown;
    }
    bool IsMouseButtonPressed(int /*button*/) const override {
        ++callsMouseButton; return mouseDown;
    }
    std::pair<float, float> GetMousePosition() const override {
        ++callsPos; return pos;
    }
    float GetMouseX() const override {
        ++callsX; return pos.first;
    }
    float GetMouseY() const override {
        ++callsY; return pos.second;
    }
};

// --- Fixture: ensures clean Input state between tests ---
class InputFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Try to clean up, even if not initialized
        Input::Shutdown();
        ASSERT_FALSE(Input::IsInitialized());
    }
    void TearDown() override {
        Input::Shutdown();
    }
};

TEST_F(InputFixture, InitializeAndShutdown_TogglesIsInitialized)
{
    EXPECT_FALSE(Input::IsInitialized());

    Input::Initialize(MakeUnique<FakeInputBackend>());
    EXPECT_TRUE(Input::IsInitialized());

    Input::Shutdown();
    EXPECT_FALSE(Input::IsInitialized());
}

TEST_F(InputFixture, DelegatesAllCallsToBackend)
{
    auto backend = MakeUnique<FakeInputBackend>();
    auto* b = backend.get();
    b->keyDown = true;
    b->mouseDown = false;
    b->pos = { 123.5f, 456.25f };

    Input::Initialize(std::move(backend));

    EXPECT_TRUE(Input::IsKeyPressed(EG_KEY_A));
    EXPECT_FALSE(Input::IsMouseButtonPressed(EG_MOUSE_BUTTON_LEFT));

    auto [x, y] = Input::GetMousePosition();
    EXPECT_FLOAT_EQ(x, 123.5f);
    EXPECT_FLOAT_EQ(y, 456.25f);

    EXPECT_FLOAT_EQ(Input::GetMouseX(), 123.5f);
    EXPECT_FLOAT_EQ(Input::GetMouseY(), 456.25f);

    // light sanity check for counters
    EXPECT_GE(b->callsKey, 1);
    EXPECT_GE(b->callsMouseButton, 1);
    EXPECT_GE(b->callsPos, 1);
    EXPECT_GE(b->callsX, 1);
    EXPECT_GE(b->callsY, 1);
}

#ifdef EG_ENABLE_CHECKS
// In Debug (with EG_ENABLE_CHECKS) the EG_CORE_CHECK macros do __debugbreak(),
// so we use GTest death tests.
TEST_F(InputFixture, Death_UsingBeforeInitializeTriggersCheck)
{
    // No Initialize()
    ASSERT_DEATH((void)Input::IsKeyPressed(EG_KEY_A), ".*");
    ASSERT_DEATH((void)Input::IsMouseButtonPressed(EG_MOUSE_BUTTON_LEFT), ".*");
    ASSERT_DEATH((void)Input::GetMousePosition(), ".*");
    ASSERT_DEATH((void)Input::GetMouseX(), ".*");
    ASSERT_DEATH((void)Input::GetMouseY(), ".*");
}

TEST_F(InputFixture, Death_DoubleInitializeTriggersCheck)
{
    Input::Initialize(MakeUnique<FakeInputBackend>());
    // Second initialization should trigger an assert
    ASSERT_DEATH(Input::Initialize(MakeUnique<FakeInputBackend>()), ".*");
}

TEST_F(InputFixture, Death_UsingAfterShutdownTriggersCheck)
{
    Input::Initialize(MakeUnique<FakeInputBackend>());
    Input::Shutdown();
    ASSERT_DEATH((void)Input::IsKeyPressed(EG_KEY_B), ".*");
}
#endif
