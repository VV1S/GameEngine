// RenderCommand.h
#pragma once
#include "RendererAPI.h"

namespace Engine {

    class RenderCommand {
    public:
        static void Init() { API()->Init(); }
        static void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) { API()->SetViewport(x, y, w, h); }
        static void SetClearColor(const glm::vec4& c) { API()->SetClearColor(c); }
        static void Clear() { API()->Clear(); }
        static void DrawIndexed(const Shared<VertexArray>& va) { API()->DrawIndexed(va); }

    private:
        static std::unique_ptr<RendererAPI>& API();
    };

} // namespace Engine
