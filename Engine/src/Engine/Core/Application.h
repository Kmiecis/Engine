#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Core/Window.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/Event.h"
#include "Engine/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Engine
{
    class Application
    {
    public:
        Application(const char* name = "Engine");
        virtual ~Application();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        void Close();

        inline Window& GetWindow() { return *m_Window; }

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        inline static Application& Get() { return *s_Instance; }

    private:
        void Run();
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}