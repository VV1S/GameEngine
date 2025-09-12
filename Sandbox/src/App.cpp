#include <Engine.h>
#include <Engine/Core/EntryPoint.h>


#include "PongLayer.h"


class GameApp final : public Engine::Application
{
public:
	GameApp()
	{
		AddLayer(std::make_shared<PongLayer>());
	}
};


Engine::Application* Engine::CreateApplication()
{
	return new GameApp();
}