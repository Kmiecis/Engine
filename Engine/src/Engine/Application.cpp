#include "enginepch.h"
#include "Application.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Log.h"

namespace Engine
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategory::EventCategoryApplication))
		{
			LOG_TRACE(e);
		}
		if (e.IsInCategory(EventCategory::EventCategoryInput))
		{
			LOG_TRACE(e);
		}

		while (true);
	}
}
