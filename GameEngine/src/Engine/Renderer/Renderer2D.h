#pragma once
#include <glm/glm.hpp>
#include "OrthographicCamera.h"
#include "Texture.h"
#include "Engine/Core/Core.h"

namespace Engine {

    class Renderer2D {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);

        static void DrawQuad(const glm::vec2& pos, const glm::vec2& size,
            const Shared<Texture2D>& texture,
            float tiling = 1.0f,
            const glm::vec4& tint = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& pos, const glm::vec2& size,
            const Shared<Texture2D>& texture,
            float tiling = 1.0f,
            const glm::vec4& tint = glm::vec4(1.0f));

        static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
            float rotation, const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
            float rotation, const glm::vec4& color);

        static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
            float rotation, const Shared<Texture2D>& texture,
            float tiling = 1.0f,
            const glm::vec4& tint = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
            float rotation, const Shared<Texture2D>& texture,
            float tiling = 1.0f,
            const glm::vec4& tint = glm::vec4(1.0f));
    };

} // namespace Engine
