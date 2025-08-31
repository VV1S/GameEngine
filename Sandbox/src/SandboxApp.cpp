#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "Platforms/OpenGL/OpenGLShader.h"

#include "GameLayer.h"
#include "ExampleLayer.h"

class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		// AddLayer(std::make_shared<ExampleLayer>());
		AddLayer(std::make_shared<GameLayer>());
	}

	~Sandbox()
	{
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}