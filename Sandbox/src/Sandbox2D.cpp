#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), 
	m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_WhiteTexture = Engine::Texture2D::Create("assets/textures/1x1.png");
	m_CheckerBoardTexture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");
	m_WhiteSubTexture = Engine::SubTexture2D::Create(m_WhiteTexture);
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Engine::Timestep timestep)
{
	// Update
	m_CameraController.OnUpdate(timestep);

	Engine::Renderer2D::ResetStats();
	// Render
	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Engine::RenderCommand::Clear();

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Engine::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_WhiteSubTexture, 1.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_WhiteSubTexture, 1.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	Engine::Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_WhiteSubTexture, 1.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	Engine::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    static bool dockspace_open = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                Engine::Application::Get().Close();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (ImGui::Begin("Settings"))
    {
        auto stats = Engine::Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));

        uint32_t textureId = m_CheckerBoardTexture->GetRendererID();
        ImGui::Image((void*)textureId, ImVec2(64.0f, 64.0f));

        ImGui::End();
    }

    ImGui::End();
}

void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
}
