#include "enginepch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

    struct Renderer2DStorage {
        Shared<VertexArray> QuadVA;
        Shared<Shader>      TextureShader;
        Shared<Texture2D>   WhiteTexture;
    };

    static Renderer2DStorage& Data() {
        static Renderer2DStorage d;
        return d;
    }

    static bool& Initialized() {
        static bool init = false;
        return init;
    }

    void Renderer2D::Init() {
        EG_PROFILE_FUNCTION();
        if (Initialized()) return;

        auto& d = Data();
        d.QuadVA = VertexArray::Create();

        const float verts[5 * 4] = {
            -0.5f,-0.5f,0.0f, 0.0f,0.0f,
             0.5f,-0.5f,0.0f, 1.0f,0.0f,
             0.5f, 0.5f,0.0f, 1.0f,1.0f,
            -0.5f, 0.5f,0.0f, 0.0f,1.0f
        };
        auto vb = VertexBuffer::Create(const_cast<float*>(verts), sizeof(verts));
        vb->SetLayout({ { ShaderDataType::Float3, "a_Position" },
                        { ShaderDataType::Float2, "a_TexCoord" } });
        d.QuadVA->AddVertexBuffer(vb);

        uint32_t idx[6] = { 0,1,2, 2,3,0 };
        auto ib = IndexBuffer::Create(idx, 6);
        d.QuadVA->SetIndexBuffer(ib);

        d.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t white = 0xffffffffu;
        d.WhiteTexture->SetData(&white, sizeof(uint32_t));

        d.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        d.TextureShader->Binding();
        d.TextureShader->SetInt("u_Texture", 0);

        Initialized() = true;
    }

    void Renderer2D::Shutdown() {
        EG_PROFILE_FUNCTION();
        if (!Initialized()) return;
        auto& d = Data();
        d.TextureShader.reset();
        d.WhiteTexture.reset();
        d.QuadVA.reset();
        Initialized() = false;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera) {
        EG_PROFILE_FUNCTION();
        Data().TextureShader->Binding();
        Data().TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene() {
        EG_PROFILE_FUNCTION();
    }

    static void SubmitQuad(const glm::mat4& transform,
        const Shared<Texture2D>& tex,
        float tiling,
        const glm::vec4& tint) {
        auto& d = Data();
        d.TextureShader->SetMat4("u_Transform", transform);
        d.TextureShader->SetFloat4("u_Color", tint);
        d.TextureShader->SetFloat("u_TilingFactor", tiling);
        tex->Bind();
        d.QuadVA->Bind();
        RenderCommand::DrawIndexed(d.QuadVA);
    }

    void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad(glm::vec3(pos, 0.0f), size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
        EG_PROFILE_FUNCTION();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
            * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
        SubmitQuad(transform, Data().WhiteTexture, 1.0f, color);
    }

    void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
        const Shared<Texture2D>& texture, float tiling, const glm::vec4& tint) {
        DrawQuad(glm::vec3(pos, 0.0f), size, texture, tiling, tint);
    }

    void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size,
        const Shared<Texture2D>& texture, float tiling, const glm::vec4& tint) {
        EG_PROFILE_FUNCTION();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
            * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
        SubmitQuad(transform, texture, tiling, tint);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
        float r, const glm::vec4& color) {
        DrawRotatedQuad(glm::vec3(pos, 0.0f), size, r, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
        float r, const glm::vec4& color) {
        EG_PROFILE_FUNCTION();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
            * glm::rotate(glm::mat4(1.0f), r, { 0,0,1 })
            * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
        SubmitQuad(transform, Data().WhiteTexture, 1.0f, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,
        float r, const Shared<Texture2D>& tex,
        float tiling, const glm::vec4& tint) {
        DrawRotatedQuad(glm::vec3(pos, 0.0f), size, r, tex, tiling, tint);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,
        float r, const Shared<Texture2D>& tex,
        float tiling, const glm::vec4& tint) {
        EG_PROFILE_FUNCTION();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
            * glm::rotate(glm::mat4(1.0f), r, { 0,0,1 })
            * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
        SubmitQuad(transform, tex, tiling, tint);
    }

} // namespace Engine
