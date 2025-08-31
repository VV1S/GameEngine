#include "enginepch.h"
#include "VertexArray.h"
#include "RendererBackend.h"

namespace Engine {

    Shared<VertexArray> VertexArray::Create() {
        EG_PROFILE_FUNCTION();
        auto fn = Detail::GetCreators().va;
        EG_CORE_CHECK(fn, "VertexArray creator not bound!");
        return fn();
    }

}
