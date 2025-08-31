#include "enginepch.h"
#include "Buffer.h"
#include "RendererBackend.h"

namespace Engine {

    Shared<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
        EG_PROFILE_FUNCTION();
        auto fn = Detail::GetCreators().vb;
        EG_CORE_CHECK(fn, "VertexBuffer creator not bound!");
        return fn(vertices, size);
    }

    Shared<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
        EG_PROFILE_FUNCTION();
        auto fn = Detail::GetCreators().ib;
        EG_CORE_CHECK(fn, "IndexBuffer creator not bound!");
        return fn(indices, count);
    }

}
