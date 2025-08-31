#pragma once

#include <string>
#include <string_view>
#include <atomic>
#include "Engine/Core/Core.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"

namespace Engine {

    // Base layer with minimal but practical metadata and lifecycle hooks.
    // Application owns the lifecycle (OnAttach/OnDetach) — LayerStack is storage only.
    class ENGINE_API Layer
    {
    public:
        // Each layer gets a stable, unique ID (monotonic counter).
        using Id = std::uint64_t;

        explicit Layer(std::string name = "Layer", int orderHint = 0) noexcept;
        virtual ~Layer() = default;

        // Lifecycle hooks (called by the Application, not by LayerStack)
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event&) {}

        // Metadata / state
        Id                  GetId()          const noexcept { return m_Id; }
        std::string_view    GetName()        const noexcept { return m_Name; }
        int                 GetOrderHint()   const noexcept { return m_OrderHint; }

        // Enable/disable controls whether Application should tick/route events to this layer.
        bool IsEnabled() const noexcept { return m_Enabled; }
        void SetEnabled(bool enabled) noexcept { m_Enabled = enabled; }

    private:
        static std::atomic<Id> s_NextId;

        Id          m_Id;
        std::string m_Name;
        int         m_OrderHint = 0;   // purely advisory; LayerStack stores insertion order separately
        bool        m_Enabled = true;
    };

} // namespace Engine
