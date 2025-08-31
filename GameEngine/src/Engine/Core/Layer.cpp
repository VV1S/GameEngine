#include "enginepch.h"
#include "Layer.h"

namespace Engine {

    std::atomic<Layer::Id> Layer::s_NextId{ 1 };

    Layer::Layer(std::string name, int orderHint) noexcept
        : m_Id{ s_NextId.fetch_add(1, std::memory_order_relaxed) }
        , m_Name(std::move(name))
        , m_OrderHint(orderHint)
        , m_Enabled(true)
    {
    }

} // namespace Engine
