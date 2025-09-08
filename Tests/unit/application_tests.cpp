    #include "TestPch.h"

    #include <gtest/gtest.h>

    #include "Engine/Core/Application.h"
    #include "Engine/Core/Layer.h"
    #include "Engine/Core/Timestep.h"
    #include "Engine/Events/Event.h"
    #include "Engine/Events/ApplicationEvent.h"

    #include "helpers/ApplicationTestAccess.h"

    using namespace Engine;

    // Test layer: counts calls; "enabled" is set via base Layer::SetEnabled API
    class CountingLayer : public Layer {
    public:
        explicit CountingLayer(const std::string& name, bool enabled = true, bool handleAnyEvent = false)
            : Layer(name), m_handleAnyEvent(handleAnyEvent)
        {
            // NOTE: using base Layer::SetEnabled (IsEnabled is not virtual)
            SetEnabled(enabled);
        }

        void OnAttach() override { ++attachCount; }
        void OnDetach() override { ++detachCount; }
        void OnUpdate(Timestep) override { ++updateCount; }
        void OnImGuiRender() override { ++imguiCount; }
        void OnEvent(Event& e) override {
            ++eventCount;
            if (m_handleAnyEvent) e.Handled = true;
        }

        int attachCount{ 0 };
        int detachCount{ 0 };
        int updateCount{ 0 };
        int imguiCount{ 0 };
        int eventCount{ 0 };

    private:
        bool m_handleAnyEvent{ false };
    };

    // Fixture: fresh Application instance for each test
    class ApplicationFixture : public ::testing::Test {
    protected:
        void SetUp() override {
            app = new Application();
        }
        void TearDown() override {
            delete app;
            app = nullptr;
        }
        Application& App() { return *app; }

        static Application* app;
    };

    Application* ApplicationFixture::app = nullptr;

    // TEST 1: UpdateLayers calls only "enabled" layers
    TEST_F(ApplicationFixture, UpdateLayers_CallsOnlyEnabledLayers) {
        auto L_enabled = std::make_shared<CountingLayer>("L_enabled", true);
        auto L_disabled = std::make_shared<CountingLayer>("L_disabled", false);

        App().AddLayer(L_enabled);
        App().AddLayer(L_disabled);

        ApplicationTestAccess::UpdateLayers(App(), Timestep{ 0.016f });

        EXPECT_EQ(L_enabled->updateCount, 1);
        EXPECT_EQ(L_disabled->updateCount, 0);
    }

    // TEST 2: Overlay is on top; event goes from top and stops at Handled
    TEST_F(ApplicationFixture, AddOverlay_ReverseOrder_StopsOnHandled) {
        auto L = std::make_shared<CountingLayer>("L", true, /*handleAnyEvent*/ false);
        auto O = std::make_shared<CountingLayer>("O", true, /*handleAnyEvent*/ true);

        App().AddLayer(L);
        App().AddOverlay(O);

        WindowCloseEvent e;
        ApplicationTestAccess::HandleEvent(App(), e);

        EXPECT_EQ(O->eventCount, 1);   // overlay received
        EXPECT_EQ(L->eventCount, 0);   // lower layer did not
        EXPECT_TRUE(e.Handled);
    }

    // TEST 3: Resize(0,0) sets minimized → UpdateLayers does nothing; after restoring >0 it works
    TEST_F(ApplicationFixture, Resize_TogglesMinimizedAndAffectsUpdates) {
        auto L = std::make_shared<CountingLayer>("L", true);
        App().AddLayer(L);

        WindowResizeEvent zero(0, 0);
        ApplicationTestAccess::CallOnWindowResize(App(), zero);
        ApplicationTestAccess::UpdateLayers(App(), Timestep{ 0.016f });
        EXPECT_EQ(L->updateCount, 0);

        WindowResizeEvent big(1280, 720);
        ApplicationTestAccess::CallOnWindowResize(App(), big);
        ApplicationTestAccess::UpdateLayers(App(), Timestep{ 0.016f });
        EXPECT_EQ(L->updateCount, 1);
    }

    // TEST 4: WindowClose handler returns true and sets loop end flag
    TEST_F(ApplicationFixture, WindowClose_HandlerReturnsTrue) {
        WindowCloseEvent e;
        bool consumed = ApplicationTestAccess::CallOnWindowClose(App(), e);
        EXPECT_TRUE(consumed);
    }
