#pragma once

#include "Engine/Core/Application.h"
#include "Engine/Debug/Log.h"

#ifdef NG_PLATFORM_WINDOWS

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