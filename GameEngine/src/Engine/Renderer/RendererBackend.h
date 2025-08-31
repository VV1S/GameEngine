#pragma once
#include <string>
#include "Engine/Core/Core.h" // Shared, MakeShared

// Forward-declare bazowe typy w Engine, ¿eby nie œci¹gaæ wszystkich nag³ówków tutaj.
namespace Engine {
    class VertexBuffer;
    class IndexBuffer;
    class VertexArray;
    class Texture2D;
    class Shader;
}

namespace Engine::Detail {

    // WskaŸniki do funkcji zwracaj¹cych Shared<...> *w przestrzeni Engine*.
    using CreateVB = Shared<::Engine::VertexBuffer>(*)(float* data, uint32_t size);
    using CreateIB = Shared<::Engine::IndexBuffer>(*)(uint32_t* indices, uint32_t count);
    using CreateVA = Shared<::Engine::VertexArray>(*)(void);
    using CreateTex = Shared<::Engine::Texture2D>(*)(uint32_t w, uint32_t h);
    using LoadTex = Shared<::Engine::Texture2D>(*)(const std::string& path);
    using LoadShader = Shared<::Engine::Shader>(*)(const std::string& path);
    using MakeShader = Shared<::Engine::Shader>(*)(const std::string& name,
        const std::string& vs,
        const std::string& fs);

    struct Creators {
        CreateVB   vb = nullptr;
        CreateIB   ib = nullptr;
        CreateVA   va = nullptr;
        CreateTex  tex = nullptr;
        LoadTex    texFromFile = nullptr;
        LoadShader shaderFromFile = nullptr;
        MakeShader shaderFromSrc = nullptr;
    };

    // Dostêp do globalnego zestawu kreatorów (jedna instancja).
    Creators& GetCreators();

    // Powi¹zanie kreatorów dla backendu OpenGL – wo³aj w Renderer::Init()
    void UseOpenGLCreators();

} // namespace Engine::Detail
