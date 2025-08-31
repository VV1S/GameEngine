#pragma once
#include "Engine/Renderer/Buffer.h"
#include <cstdint>

namespace Engine {

    class OpenGLVertexBuffer final : public VertexBuffer {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;
        OpenGLVertexBuffer(const OpenGLVertexBuffer&) = delete;
        OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer&) = delete;
        OpenGLVertexBuffer(OpenGLVertexBuffer&&) noexcept;
        OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&&) noexcept;

        void Bind() const override;
        void Unbind() const override;

        const BufferLayout& GetLayout() const override { return m_Layout; }
        void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

        uint32_t id() const noexcept { return m_ID; }

    private:
        uint32_t m_ID = 0;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer final : public IndexBuffer {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer() override;
        OpenGLIndexBuffer(const OpenGLIndexBuffer&) = delete;
        OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer&) = delete;
        OpenGLIndexBuffer(OpenGLIndexBuffer&&) noexcept;
        OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&&) noexcept;

        void Bind() const override;
        void Unbind() const override;

        uint32_t GetCount() const override { return m_Count; }
        uint32_t id() const noexcept { return m_ID; }

    private:
        uint32_t m_ID = 0;
        uint32_t m_Count = 0;
    };

} // namespace Engine
