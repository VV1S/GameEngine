// RendererAPI.h
#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include "Engine/Core/Core.h"
#include "VertexArray.h"

namespace Engine {

    class RendererAPI {
    public:
        enum class API { None = 0, OpenGL = 1 };
        virtual ~RendererAPI() = default;

        virtual void Init() = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const Shared<VertexArray>& va) = 0;

        static API GetAPI() { return s_API; }
    protected:
        static API s_API;
    };

} // namespace Engine
