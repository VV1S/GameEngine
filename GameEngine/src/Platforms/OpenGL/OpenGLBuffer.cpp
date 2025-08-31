#include "enginepch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Engine {

    // -------- VertexBuffer -------------------------------------------------------

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
        EG_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_ID);
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        EG_PROFILE_FUNCTION();
        if (m_ID) glDeleteBuffers(1, &m_ID);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept
        : m_ID(other.m_ID), m_Layout(std::move(other.m_Layout)) {
        other.m_ID = 0;
    }

    OpenGLVertexBuffer& OpenGLVertexBuffer::operator=(OpenGLVertexBuffer&& other) noexcept {
        if (this != &other) {
            if (m_ID) glDeleteBuffers(1, &m_ID);
            m_ID = other.m_ID;
            m_Layout = std::move(other.m_Layout);
            other.m_ID = 0;
        }
        return *this;
    }

    void OpenGLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    }

    void OpenGLVertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // -------- IndexBuffer --------------------------------------------------------

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
        : m_Count(count) {
        EG_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        EG_PROFILE_FUNCTION();
        if (m_ID) glDeleteBuffers(1, &m_ID);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept
        : m_ID(other.m_ID), m_Count(other.m_Count) {
        other.m_ID = 0;
        other.m_Count = 0;
    }

    OpenGLIndexBuffer& OpenGLIndexBuffer::operator=(OpenGLIndexBuffer&& other) noexcept {
        if (this != &other) {
            if (m_ID) glDeleteBuffers(1, &m_ID);
            m_ID = other.m_ID;
            m_Count = other.m_Count;
            other.m_ID = 0;
            other.m_Count = 0;
        }
        return *this;
    }

    void OpenGLIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

    void OpenGLIndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

} // namespace Engine
