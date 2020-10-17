#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS

#include "Engine/Core/Application.h"
#include "Engine/Debug/Log.h"

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Log::Init();

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#else
	#error Define supported platform
#endif
