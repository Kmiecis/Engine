#pragma once

#include "Engine/Core.h"
#include "Engine/Events/Event.h"
#include "Engine/Window.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/LayerStack.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		
		inline static Application& Get() { return *s_Instance; }
		
	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
