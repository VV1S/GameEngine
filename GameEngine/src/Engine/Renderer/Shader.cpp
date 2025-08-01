#include "enginepch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace Engine {

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}