#pragma once

#ifdef EG_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Log::Init();
	EG_CORE_WARN("Initialized Log!");
	int a = 5;
	EG_INFO("Hello Var={0}", a);

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}

#endif
