#include "enginepch.h"
#include "RendererBackend.h"

#include "Platforms/OpenGL/OpenGLBuffer.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"
#include "Platforms/OpenGL/OpenGLTexture.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace Engine::Detail {

    static Creators& S() {
        static Creators c{};
        return c;
    }

    Creators& GetCreators() { return S(); }

    // ---- OpenGL bindings (one-time) ----
    static Shared<VertexBuffer>  GL_CreateVB(float* d, uint32_t s) {
        return MakeShared<OpenGLVertexBuffer>(d, s);
    }
    static Shared<IndexBuffer>   GL_CreateIB(uint32_t* idx, uint32_t cnt) {
        return MakeShared<OpenGLIndexBuffer>(idx, cnt);
    }
    static Shared<VertexArray>   GL_CreateVA() {
        return MakeShared<OpenGLVertexArray>();
    }
    static Shared<Texture2D>     GL_CreateTex(uint32_t w, uint32_t h) {
        return MakeShared<OpenGLTexture2D>(w, h);
    }
    static Shared<Texture2D>     GL_LoadTex(const std::string& path) {
        return MakeShared<OpenGLTexture2D>(path);
    }
    static Shared<Shader>        GL_LoadShader(const std::string& path) {
        return MakeShared<OpenGLShader>(path);
    }
    static Shared<Shader>        GL_MakeShader(const std::string& name,
        const std::string& vs,
        const std::string& fs) {
        return MakeShared<OpenGLShader>(name, vs, fs);
    }

    void UseOpenGLCreators() {
        auto& c = S();
        c.vb = &GL_CreateVB;
        c.ib = &GL_CreateIB;
        c.va = &GL_CreateVA;
        c.tex = &GL_CreateTex;
        c.texFromFile = &GL_LoadTex;
        c.shaderFromFile = &GL_LoadShader;
        c.shaderFromSrc = &GL_MakeShader;
    }

} // namespace Engine::Detail
