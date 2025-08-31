#include "enginepch.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Renderer.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace Engine {

    Shared<Shader> Shader::Create(const std::string& filepath)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            EG_CORE_CHECK(false, "RendererAPI::None is not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return MakeShared<OpenGLShader>(filepath);
        default:
            EG_CORE_CHECK(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }

    Shared<Shader> Shader::Create(const std::string& name,
        const std::string& vertexSrc,
        const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            EG_CORE_CHECK(false, "RendererAPI::None is not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return MakeShared<OpenGLShader>(name, vertexSrc, fragmentSrc);
        default:
            EG_CORE_CHECK(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }

    void ShaderLibrary::Add(const std::string& name, const Shared<Shader>& shader)
    {
        EG_CORE_CHECK(!Exists(name), "Shader '{}' already exists!", name);
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Shared<Shader>& shader)
    {
        const std::string& name = shader->GetName();
        Add(name, shader);
    }

    Shared<Shader> ShaderLibrary::Load(const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Shared<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Shared<Shader> ShaderLibrary::Get(const std::string& name)
    {
        EG_CORE_CHECK(Exists(name), "Shader '{}' not found!", name);
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }

} // namespace Engine
