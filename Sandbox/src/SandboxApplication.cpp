#include <Engine.h>

class SandboxApplication : public Engine::Application
{
public:
	SandboxApplication()
	{
	}

	~SandboxApplication()
	{
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new SandboxApplication();
}
