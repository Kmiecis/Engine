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
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_TAB))
		{
			LOG_TRACE("Tab key is pressed!");
		}
	}

	void OnEvent(Engine::Event& event) override
	{
		if (event.GetEventType() == Engine::EventType::KeyPressed)
		{
			Engine::KeyPressedEvent& e = (Engine::KeyPressedEvent&)event;
			LOG_TRACE((char)e.GetKeyCode());
		}
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
