#pragma once
#include "Engine/Renderer/RendererAPI.h"

namespace Engine {

    class OpenGLRendererAPI final : public RendererAPI {
    public:
        void Init() override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawIndexed(const std::shared_ptr<VertexArray>& va) override;
    };

} // namespace Engine
