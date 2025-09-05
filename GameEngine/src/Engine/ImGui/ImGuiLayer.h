#pragma once

#include <string>
#include "Engine/Core/Layer.h"
#include "Engine/Events/Event.h"
#include "../Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

struct ImFont; // fwd

namespace Engine {

    class ENGINE_API ImGuiLayer : public Layer
    {
    public:
        struct Options {
            bool EnableDocking = true;
            bool EnableViewports = true;
        };

        explicit ImGuiLayer(const Options& opts = {});
        ~ImGuiLayer() override;

        // Layer
        void OnAttach() override;   
        void OnDetach() override;   

        // Per-frame
        void Begin();
        void End();

        // Configuration helpers
        void SetDockingEnabled(bool enable) { m_Opts.EnableDocking = enable; }
        void SetViewportsEnabled(bool enable) { m_Opts.EnableViewports = enable; }

        // Fonts
        ImFont* AddFontFromFile(const char* path, float sizePixels, bool setAsDefault = false);

        // Theme
        static void ApplyDarkTheme();

    private:
        bool   m_Initialized = false;
        Options m_Opts{};
    };

} // namespace Engine
