#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS

#include "Application.h"

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Engine::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#else
	#error Define supported platform
#endif