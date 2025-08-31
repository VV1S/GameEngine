#include "enginepch.h"
#include "Texture.h"
#include "RendererBackend.h"

namespace Engine {

    Shared<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
        EG_PROFILE_FUNCTION();
        auto fn = Detail::GetCreators().tex;
        EG_CORE_CHECK(fn, "Texture2D (size) creator not bound!");
        return fn(width, height);
    }

    Shared<Texture2D> Texture2D::Create(const std::string& path) {
        EG_PROFILE_FUNCTION();
        auto fn = Detail::GetCreators().texFromFile;
        EG_CORE_CHECK(fn, "Texture2D (file) creator not bound!");
        return fn(path);
    }

}
