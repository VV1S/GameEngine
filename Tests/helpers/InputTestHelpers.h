#pragma once
#include <unordered_set>
#include <utility>
#include "Engine/Core/Input.h"

namespace TestHelpers {

    class FakeInputBackend : public Engine::InputBackend {
    public:
        ~FakeInputBackend() override = default;

        bool IsKeyPressed(int keycode) const override { return keys_.count(keycode) != 0; }
        bool IsMouseButtonPressed(int) const override { return false; }
        std::pair<float, float> GetMousePosition() const override { return { 0.f, 0.f }; }
        float GetMouseX() const override { return 0.f; }
        float GetMouseY() const override { return 0.f; }

        void press(int k) { keys_.insert(k); }
        void release(int k) { keys_.erase(k); }
        void clear() { keys_.clear(); }

    private:
        std::unordered_set<int> keys_;
    };

    // RAII only for tests WITHOUT fixture
    class InputGuard {
    public:
        explicit InputGuard(std::unique_ptr<Engine::InputBackend> b)
        {
            Engine::Input::Initialize(std::move(b));
        }
        ~InputGuard() {
            Engine::Input::Shutdown();
        }

        InputGuard(const InputGuard&) = delete;
        InputGuard& operator=(const InputGuard&) = delete;
    };

} // namespace TestHelpers
