#pragma once
#include "Engine/Core/Core.h"
#include "Engine/Renderer/VertexArray.h"
#include <vector>
#include <cstdint>

namespace Engine {

    class OpenGLVertexArray final : public VertexArray {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        void Bind() const override;
        void Unbind() const override;

        void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer) override;
        void SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer) override;

        const std::vector<Shared<VertexBuffer>>& GetVertexBuffers() const override { return m_VBs; }
        const Shared<IndexBuffer>& GetIndexBuffer() const override { return m_IB; }

    private:
        uint32_t m_VAO = 0;
        std::vector<Shared<VertexBuffer>> m_VBs;
        Shared<IndexBuffer> m_IB;

        uint32_t m_BindingBase = 0;
        uint32_t m_AttribBase = 0;
    };

} // namespace Engine
