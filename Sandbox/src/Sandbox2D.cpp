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
    m_CheckerBoardTexture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");
    m_WhiteTexture = Engine::Texture2D::Create("assets/textures/1x1.png");
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
    if (ImGui::Begin("Settings"))
    {
        auto stats = Engine::Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

        ImGui::End();
    }
}

void Sandbox2D::OnEvent(Engine::Event& event)
{
    m_CameraController.OnEvent(event);
}
