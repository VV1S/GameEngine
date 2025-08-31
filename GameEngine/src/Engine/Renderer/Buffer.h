#pragma once
#include "Engine/Core/Core.h"
#include <vector>
#include <string>
#include <cstdint>

namespace Engine {

    enum class ShaderDataType {
        None = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool
    };

    inline uint32_t ShaderDataTypeSize(ShaderDataType t) {
        switch (t) {
        case ShaderDataType::Float:  return 4u;
        case ShaderDataType::Float2: return 8u;
        case ShaderDataType::Float3: return 12u;
        case ShaderDataType::Float4: return 16u;
        case ShaderDataType::Mat3:   return 36u;
        case ShaderDataType::Mat4:   return 64u;
        case ShaderDataType::Int:    return 4u;
        case ShaderDataType::Int2:   return 8u;
        case ShaderDataType::Int3:   return 12u;
        case ShaderDataType::Int4:   return 16u;
        case ShaderDataType::Bool:   return 1u;
        default: EG_CORE_CHECK(false, "Unknown ShaderDataType!"); return 0u;
        }
    }

    struct BufferElement {
        std::string   Name;
        ShaderDataType Type = ShaderDataType::None;
        uint32_t      Size = 0;
        uint32_t      Offset = 0;
        bool          Normalized = false;

        BufferElement() = default;
        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Normalized(normalized) {
        }
        uint32_t GetComponentCount() const {
            switch (Type) {
            case ShaderDataType::Float:  return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3:   return 9;
            case ShaderDataType::Mat4:   return 16;
            case ShaderDataType::Int:    return 1;
            case ShaderDataType::Int2:   return 2;
            case ShaderDataType::Int3:   return 3;
            case ShaderDataType::Int4:   return 4;
            case ShaderDataType::Bool:   return 1;
            default: EG_CORE_CHECK(false, "Unknown ShaderDataType!"); return 0;
            }
        }
    };

    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_Elements(elements) {
            Recalculate();
        }

        const std::vector<BufferElement>& Elements() const { return m_Elements; }
        uint32_t Stride() const { return m_Stride; }

        auto begin() { return m_Elements.begin(); }
        auto end() { return m_Elements.end(); }
        auto begin() const { return m_Elements.begin(); }
        auto end()   const { return m_Elements.end(); }

    private:
        void Recalculate() {
            m_Stride = 0;
            uint32_t offset = 0;
            for (auto& e : m_Elements) {
                e.Offset = offset;
                offset += e.Size;
                m_Stride += e.Size;
            }
        }
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    class ENGINE_API VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;
        virtual void Bind()   const = 0;
        virtual void Unbind() const = 0;
        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;

        static Shared<VertexBuffer> Create(float* vertices, uint32_t size);
    };

    class ENGINE_API IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;
        virtual void Bind()   const = 0;
        virtual void Unbind() const = 0;
        virtual uint32_t GetCount() const = 0;

        static Shared<IndexBuffer> Create(uint32_t* indices, uint32_t count);
    };

} // namespace Engine
