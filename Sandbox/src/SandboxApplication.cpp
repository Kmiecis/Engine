#include <Engine.h>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		LOG_INFO("ExampleLayer::OnUpdate");
	}

	void OnEvent(Engine::Event& event) override
	{
		LOG_TRACE("{0}", event);
	}

};

class SandboxApplication : public Engine::Application
{
public:
	SandboxApplication()
	{
		PushLayer(new ExampleLayer());
	}

	~SandboxApplication()
	{
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new SandboxApplication();
}
