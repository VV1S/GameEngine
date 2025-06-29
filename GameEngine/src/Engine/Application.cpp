#include "enginepch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace Engine {

	Application::Application()
	{
		EG_CORE_INFO("Application::Constructor start");
		m_Window = std::unique_ptr<Window>(Window::Create());
		EG_CORE_INFO("Application::Constructor end");
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		EG_CORE_INFO("Application::Run() start");
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
		EG_CORE_INFO("Application::Run() end");
	}
}