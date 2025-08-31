#include "enginepch.h"
#include "LayerStack.h"

namespace Engine {

    LayerStack::LayerStack() = default;
    LayerStack::~LayerStack() = default;

    void LayerStack::AddLayer(const std::shared_ptr<Layer>& layer)
    {
        EG_CORE_CHECK(layer, "Attempted to add a null Layer.");
        // Store only; lifecycle is caller's responsibility (Application)
        m_Layers.insert(m_Layers.begin() + static_cast<std::ptrdiff_t>(m_InsertIndex), layer);
        ++m_InsertIndex;
    }

    void LayerStack::AddOverlay(const std::shared_ptr<Layer>& overlay)
    {
        EG_CORE_CHECK(overlay, "Attempted to add a null Overlay.");
        // Store only; lifecycle is caller's responsibility (Application)
        m_Layers.emplace_back(overlay);
    }

    void LayerStack::RemoveLayer(const std::shared_ptr<Layer>& layer)
    {
        if (!layer) return;
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
        {
            const auto idx = static_cast<std::size_t>(std::distance(m_Layers.begin(), it));
            m_Layers.erase(it);
            if (idx < m_InsertIndex && m_InsertIndex > 0)
                --m_InsertIndex;
        }
    }

    void LayerStack::RemoveOverlay(const std::shared_ptr<Layer>& overlay)
    {
        if (!overlay) return;
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            const auto idx = static_cast<std::size_t>(std::distance(m_Layers.begin(), it));
            m_Layers.erase(it);
            // Only adjust insert index if we accidentally removed from the "layer region"
            if (idx < m_InsertIndex && m_InsertIndex > 0)
                --m_InsertIndex;
        }
    }

    bool LayerStack::RemoveById(Layer::Id id)
    {
        auto it = std::find_if(m_Layers.begin(), m_Layers.end(),
            [id](const std::shared_ptr<Layer>& l) { return l && l->GetId() == id; });
        if (it == m_Layers.end()) return false;

        const auto idx = static_cast<std::size_t>(std::distance(m_Layers.begin(), it));
        m_Layers.erase(it);
        if (idx < m_InsertIndex && m_InsertIndex > 0)
            --m_InsertIndex;
        return true;
    }

    bool LayerStack::RemoveByName(std::string_view name)
    {
        auto it = std::find_if(m_Layers.begin(), m_Layers.end(),
            [name](const std::shared_ptr<Layer>& l) { return l && l->GetName() == name; });
        if (it == m_Layers.end()) return false;

        const auto idx = static_cast<std::size_t>(std::distance(m_Layers.begin(), it));
        m_Layers.erase(it);
        if (idx < m_InsertIndex && m_InsertIndex > 0)
            --m_InsertIndex;
        return true;
    }

    std::shared_ptr<Layer> LayerStack::FindById(Layer::Id id) const
    {
        auto it = std::find_if(m_Layers.begin(), m_Layers.end(),
            [id](const std::shared_ptr<Layer>& l) { return l && l->GetId() == id; });
        return (it != m_Layers.end()) ? *it : nullptr;
    }

    std::shared_ptr<Layer> LayerStack::FindByName(std::string_view name) const
    {
        auto it = std::find_if(m_Layers.begin(), m_Layers.end(),
            [name](const std::shared_ptr<Layer>& l) { return l && l->GetName() == name; });
        return (it != m_Layers.end()) ? *it : nullptr;
    }

    void LayerStack::Clear()
    {
        // Storage only: Application should have called OnDetach() if needed.
        m_Layers.clear();
        m_InsertIndex = 0;
    }

} // namespace Engine
