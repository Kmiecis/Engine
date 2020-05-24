#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS

#include "Application.h"
#include "Log.h"

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Log::Init();

	LOG_CORE_WARN("Initialized Log");
	int a = 5;
	LOG_INFO("Hello. a = {0}", a);

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#else
	#error Define supported platform
#endif
