#include "enginepch.h"
#include "OpenGLVertexArray.h"
#include "Engine/Renderer/Buffer.h"
#include "Platforms/OpenGL/OpenGLBuffer.h"
#include <glad/glad.h>

namespace Engine {

    static GLenum ToGLType(ShaderDataType t) {
        switch (t) {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:  return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:  return GL_INT;
        case ShaderDataType::Bool:  return GL_BOOL;
        default: EG_CORE_CHECK(false, "Unknown ShaderDataType"); return GL_FLOAT;
        }
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        EG_PROFILE_FUNCTION();
        glCreateVertexArrays(1, &m_VAO);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    }

    void OpenGLVertexArray::Bind() const { glBindVertexArray(m_VAO); }
    void OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }

    void OpenGLVertexArray::AddVertexBuffer(const Shared<VertexBuffer>& vb) {
        EG_PROFILE_FUNCTION();

        const auto& layout = vb->GetLayout();
        EG_CORE_CHECK(!layout.Elements().empty(), "VertexBuffer has no layout");

        // pobierz GL id bufora
        auto* glVB = dynamic_cast<OpenGLVertexBuffer*>(vb.get());
        EG_CORE_CHECK(glVB, "VertexBuffer is not OpenGLVertexBuffer");
        const GLuint vbo = glVB->id();

        const GLuint binding = (GLuint)m_BindingBase++;
        glVertexArrayVertexBuffer(m_VAO, binding, vbo, 0, (GLsizei)layout.Stride());

        GLuint attrib = (GLuint)m_AttribBase;

        for (const BufferElement& e : layout.Elements()) {
            const GLenum base = ToGLType(e.Type);
            const GLboolean norm = e.Normalized ? GL_TRUE : GL_FALSE;

            switch (e.Type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
                glEnableVertexArrayAttrib(m_VAO, attrib);
                glVertexArrayAttribBinding(m_VAO, attrib, binding);
                glVertexArrayAttribFormat(m_VAO, attrib, (GLint)e.GetComponentCount(), base, norm, (GLuint)e.Offset);
                attrib++;
                break;

            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool:
                glEnableVertexArrayAttrib(m_VAO, attrib);
                glVertexArrayAttribBinding(m_VAO, attrib, binding);
                glVertexArrayAttribIFormat(m_VAO, attrib, (GLint)e.GetComponentCount(), base, (GLuint)e.Offset);
                attrib++;
                break;

            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4: {
                const uint32_t cols = (e.Type == ShaderDataType::Mat3) ? 3u : 4u;
                const uint32_t colSize = (uint32_t)sizeof(float) * cols;

                for (uint32_t c = 0; c < cols; ++c) {
                    glEnableVertexArrayAttrib(m_VAO, attrib);
                    glVertexArrayAttribBinding(m_VAO, attrib, binding);
                    glVertexArrayAttribFormat(m_VAO, attrib, (GLint)cols, GL_FLOAT, norm,
                        (GLuint)(e.Offset + c * colSize));
                    attrib++;
                }
            } break;

            default:
                EG_CORE_CHECK(false, "Unhandled ShaderDataType");
            }
        }

        m_AttribBase = (uint32_t)attrib;
        m_VBs.push_back(vb);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Shared<IndexBuffer>& ib) {
        EG_PROFILE_FUNCTION();

        auto* glIB = dynamic_cast<OpenGLIndexBuffer*>(ib.get());
        EG_CORE_CHECK(glIB, "IndexBuffer is not OpenGLIndexBuffer");

        glVertexArrayElementBuffer(m_VAO, glIB->id());
        m_IB = ib;
    }

} // namespace Engine
