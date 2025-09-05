#pragma once

#include <vector>
#include <memory>
#include <string_view>
#include <optional>
#include <algorithm>
#include "Engine/Core/Core.h"
#include "Layer.h"

namespace Engine {

    // - Pure storage: NO OnAttach/OnDetach here (Application handles lifecycle).
    // - Maintains normal layers first, then overlays after m_InsertIndex.
    // - Offers convenience removals and lookups by id/name.
    class ENGINE_API LayerStack
    {
    public:
        using Container = std::vector<std::shared_ptr<Layer>>;
        using iterator = Container::iterator;
        using const_iterator = Container::const_iterator;
        using reverse_iterator = Container::reverse_iterator;
        using const_reverse_iterator = Container::const_reverse_iterator;

        LayerStack();
        ~LayerStack();

        // Add regular layer at the insertion index (before overlays).
        void AddLayer(const std::shared_ptr<Layer>& layer);
        // Add overlay at the end (always above normal layers).
        void AddOverlay(const std::shared_ptr<Layer>& overlay);

        // Remove by pointer (no lifecycle here).
        void RemoveLayer(const std::shared_ptr<Layer>& layer);
        void RemoveOverlay(const std::shared_ptr<Layer>& overlay);

        // Convenience removals/lookups
        bool RemoveById(Layer::Id id);
        bool RemoveByName(std::string_view name);

        std::shared_ptr<Layer> FindById(Layer::Id id) const;
        std::shared_ptr<Layer> FindByName(std::string_view name) const;

        // Clear all (storage only; Application should have called OnDetach).
        void Clear();

        // Iteration
        iterator begin() noexcept { return m_Layers.begin(); }
        iterator end()   noexcept { return m_Layers.end(); }
        const_iterator begin() const noexcept { return m_Layers.begin(); }
        const_iterator end()   const noexcept { return m_Layers.end(); }

        reverse_iterator rbegin() noexcept { return m_Layers.rbegin(); }
        reverse_iterator rend()   noexcept { return m_Layers.rend(); }
        const_reverse_iterator rbegin() const noexcept { return m_Layers.rbegin(); }
        const_reverse_iterator rend()   const noexcept { return m_Layers.rend(); }

        // Introspection
        std::size_t Size() const noexcept { return m_Layers.size(); }
        std::size_t LayerCount() const noexcept { return m_InsertIndex; } // normal (non-overlay) count

    private:
        Container   m_Layers;
        std::size_t m_InsertIndex = 0; // index where overlays start
    };

} // namespace Engine
