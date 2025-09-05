#include "enginepch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "RendererBackend.h"

namespace Engine {

    Renderer::SceneData& Renderer::Scene() {
        static SceneData s;
        return s;
    }

    void Renderer::Init() {
        EG_PROFILE_FUNCTION();
        RenderCommand::Init();

        // Bind creators ONCE based on current API
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            Detail::UseOpenGLCreators();
            break;
        case RendererAPI::API::None:
        default:
            EG_CORE_CHECK(false, "Unsupported Renderer API!");
            break;
        }

        Renderer2D::Init();
    }

    void Renderer::Shutdown() {
        EG_PROFILE_FUNCTION();
        Renderer2D::Shutdown();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(OrthographicCamera& camera) {
        Scene().ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {
        // place for future flush/batch
    }

    void Renderer::Submit(const Shared<Shader>& shader,
        const Shared<VertexArray>& vertexArray,
        const glm::mat4& transform) {
        EG_PROFILE_FUNCTION();
        shader->Bind();
        shader->SetMat4("u_ViewProjection", Scene().ViewProjectionMatrix);
        shader->SetMat4("u_Transform", transform);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

} // namespace Engine
