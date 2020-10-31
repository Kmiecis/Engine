#include <Engine/Core/EntryPoint.h>

#include "Sandbox2D.h"

class SandboxApplication : public Engine::Application
{
public:
	SandboxApplication()
	{
		PushLayer(new Sandbox2D());
	}

	~SandboxApplication()
	{
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new SandboxApplication();
}