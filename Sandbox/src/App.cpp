#include <Engine.h>
#include <Engine/Core/EntryPoint.h>


#include "PlaySceneLayer.h"


class GameApp final : public Engine::Application
{
public:
	GameApp()
	{
		AddLayer(std::make_shared<PlaySceneLayer>());
	}
};


Engine::Application* Engine::CreateApplication()
{
	return new GameApp();
}