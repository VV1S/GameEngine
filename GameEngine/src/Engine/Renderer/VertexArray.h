#pragma once
#include <vector>
#include "Engine/Core/Core.h"
#include "Engine/Renderer/Buffer.h"

namespace Engine {

    class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind()   const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Shared<VertexBuffer>& vb) = 0;
        virtual void SetIndexBuffer(const Shared<IndexBuffer>& ib) = 0;

        virtual const std::vector<Shared<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const Shared<IndexBuffer>& GetIndexBuffer() const = 0;

        static Shared<VertexArray> Create();
    };

} // namespace Engine
