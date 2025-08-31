#include "enginepch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Engine {

    void OpenGLRendererAPI::Init() {
        EG_PROFILE_FUNCTION();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
        glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& c) {
        glClearColor(c.r, c.g, c.b, c.a);
    }

    void OpenGLRendererAPI::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& va) {
        glDrawElements(GL_TRIANGLES, (GLsizei)va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

} // namespace Engine
