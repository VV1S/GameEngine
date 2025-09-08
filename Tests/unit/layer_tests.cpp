#include <gtest/gtest.h>

#include "Engine/Core/Layer.h"
#include "Engine/Core/LayerStack.h"

using namespace Engine;

namespace {

    class TestLayer : public Layer {
    public:
        explicit TestLayer(std::string name, int orderHint = 0)
            : Layer(std::move(name), orderHint) {
        }

        void OnAttach() override { attached = true; }
        void OnDetach() override { detached = true; }
        void OnUpdate(Timestep) override { ++updates; }
        void OnEvent(Event&) override { ++events; }

        bool attached = false;
        bool detached = false;
        int  updates = 0;
        int  events = 0;
    };

} // namespace

// ---------- Layer: podstawy ----------

TEST(Layer_Basics, DefaultsAndIdMonotonicAndEnableFlag)
{
    TestLayer a{ "A", /*orderHint*/5 };
    TestLayer b{ "B", /*orderHint*/-2 };

    // name / orderHint
    EXPECT_EQ(a.GetName(), std::string_view("A"));
    EXPECT_EQ(a.GetOrderHint(), 5);
    EXPECT_EQ(b.GetName(), std::string_view("B"));
    EXPECT_EQ(b.GetOrderHint(), -2);

    EXPECT_TRUE(a.IsEnabled());
    a.SetEnabled(false);
    EXPECT_FALSE(a.IsEnabled());
    a.SetEnabled(true);
    EXPECT_TRUE(a.IsEnabled());

    const auto idA = a.GetId();
    const auto idB = b.GetId();
    EXPECT_NE(idA, idB);
    EXPECT_LT(idA, idB);
}

// ---------- LayerStack: dodawanie i kolejnoœæ ----------

TEST(LayerStack_Basics, AddLayerAndOverlay_OrderIsLayersThenOverlays)
{
    LayerStack stack;

    auto l1 = std::make_shared<TestLayer>("L1");
    auto ov = std::make_shared<TestLayer>("OV");
    auto l2 = std::make_shared<TestLayer>("L2");

    stack.AddLayer(l1);    
    stack.AddOverlay(ov); 
    stack.AddLayer(l2);    

    ASSERT_EQ(stack.Size(), 3u);
    EXPECT_EQ(stack.LayerCount(), 2u); // tylko zwyk³e warstwy

    auto it = stack.begin();
    ASSERT_NE(it, stack.end());
    EXPECT_EQ((*it++)->GetName(), "L1");
    ASSERT_NE(it, stack.end());
    EXPECT_EQ((*it++)->GetName(), "L2");
    ASSERT_NE(it, stack.end());
    EXPECT_EQ((*it++)->GetName(), "OV");
    EXPECT_EQ(it, stack.end());
}

TEST(LayerStack_Remove, RemoveLayerAdjustsInsertIndexAndKeepsOrder)
{
    LayerStack stack;

    auto l1 = std::make_shared<TestLayer>("L1");
    auto l2 = std::make_shared<TestLayer>("L2");
    auto ov = std::make_shared<TestLayer>("OV");

    stack.AddLayer(l1);   // [L1]
    stack.AddLayer(l2);   // [L1, L2]
    stack.AddOverlay(ov); // [L1, L2, OV]
    EXPECT_EQ(stack.LayerCount(), 2u);

    stack.RemoveLayer(l1); // -> [L2, OV]
    ASSERT_EQ(stack.Size(), 2u);
    EXPECT_EQ(stack.LayerCount(), 1u);

    auto l3 = std::make_shared<TestLayer>("L3");
    stack.AddLayer(l3); // -> [L2, L3, OV]
    ASSERT_EQ(stack.Size(), 3u);
    EXPECT_EQ(stack.LayerCount(), 2u);

    auto names = std::vector<std::string>();
    for (auto& p : stack) names.emplace_back(p->GetName());
    EXPECT_EQ((names), (std::vector<std::string>{"L2", "L3", "OV"}));
}

TEST(LayerStack_Remove, RemoveOverlayDoesNotChangeLayerCount)
{
    LayerStack stack;

    auto l1 = std::make_shared<TestLayer>("L1");
    auto ov1 = std::make_shared<TestLayer>("OV1");
    auto ov2 = std::make_shared<TestLayer>("OV2");

    stack.AddLayer(l1);    // [L1]
    stack.AddOverlay(ov1); // [L1, OV1]
    stack.AddOverlay(ov2); // [L1, OV1, OV2]
    EXPECT_EQ(stack.LayerCount(), 1u);

    stack.RemoveOverlay(ov1); // [L1, OV2]
    EXPECT_EQ(stack.LayerCount(), 1u);

    auto l2 = std::make_shared<TestLayer>("L2");
    stack.AddLayer(l2); // -> [L1, L2, OV2]
    ASSERT_EQ(stack.Size(), 3u);
    EXPECT_EQ(stack.LayerCount(), 2u);

    auto names = std::vector<std::string>();
    for (auto& p : stack) names.emplace_back(p->GetName());
    EXPECT_EQ((names), (std::vector<std::string>{"L1", "L2", "OV2"}));
}

TEST(LayerStack_FindRemove, FindByIdAndNameThenRemove)
{
    LayerStack stack;

    auto a = std::make_shared<TestLayer>("A");
    auto b = std::make_shared<TestLayer>("B");
    auto c = std::make_shared<TestLayer>("C");

    stack.AddLayer(a);
    stack.AddLayer(b);
    stack.AddOverlay(c);

    // Find by name
    auto fb = stack.FindByName("B");
    ASSERT_NE(fb, nullptr);
    EXPECT_EQ(fb->GetName(), "B");

    // Find by id
    auto idC = c->GetId();
    auto fc = stack.FindById(idC);
    ASSERT_NE(fc, nullptr);
    EXPECT_EQ(fc->GetName(), "C");

    // Remove by id
    EXPECT_TRUE(stack.RemoveById(idC));
    EXPECT_EQ(stack.FindById(idC), nullptr);

    // Remove by name
    EXPECT_TRUE(stack.RemoveByName("A"));
    EXPECT_EQ(stack.FindByName("A"), nullptr);

    // End state
    ASSERT_EQ(stack.Size(), 1u);
    ASSERT_EQ(stack.begin()->get(), b.get());
    EXPECT_EQ(stack.LayerCount(), 1u);
}


TEST(LayerStack_Iterators, ForwardAndReverseAndClear)
{
    LayerStack stack;

    auto l1 = std::make_shared<TestLayer>("L1");
    auto l2 = std::make_shared<TestLayer>("L2");
    auto ov = std::make_shared<TestLayer>("OV");

    stack.AddLayer(l1);
    stack.AddLayer(l2);
    stack.AddOverlay(ov);

    // forward
    {
        std::vector<std::string> fwd;
        for (auto it = stack.begin(); it != stack.end(); ++it)
            fwd.emplace_back((*it)->GetName());
        EXPECT_EQ((fwd), (std::vector<std::string>{"L1", "L2", "OV"}));
    }

    // reverse
    {
        std::vector<std::string> rev;
        for (auto it = stack.rbegin(); it != stack.rend(); ++it)
            rev.emplace_back((*it)->GetName());
        EXPECT_EQ((rev), (std::vector<std::string>{"OV", "L2", "L1"}));
    }

    // Clear()
    stack.Clear();
    EXPECT_EQ(stack.Size(), 0u);
    EXPECT_EQ(stack.LayerCount(), 0u);
    EXPECT_EQ(stack.begin(), stack.end());
}
