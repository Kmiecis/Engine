#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_WhiteTexture = Engine::Texture2D::Create("assets/textures/1x1.png");
	m_CheckerBoardTexture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Engine::Timestep timestep)
{
	// Update
	m_CameraController.OnUpdate(timestep);

	// Render
	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Engine::RenderCommand::Clear();

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	//Engine::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(45.0f), m_WhiteTexture, 1.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
	Engine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_WhiteTexture, 1.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
	Engine::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_WhiteTexture, 1.0f, { 0.2f, 0.3f, 0.8f, 1.0f });
	Engine::Renderer2D::DrawQuad({ -5.0f, -5.0f, -0.1f }, { 10.5f, 10.5f }, m_CheckerBoardTexture, 10.0f);
	Engine::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
}
