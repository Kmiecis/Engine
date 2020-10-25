#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), 
	m_CameraController(1280.0f / 720.0f),
	m_ParticleSystem(1000)
{
}

void Sandbox2D::OnAttach()
{
	m_WhiteTexture = Engine::Texture2D::Create("assets/textures/1x1.png");
	m_CheckerBoardTexture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");

	m_ParticleProperties.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_ParticleProperties.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_ParticleProperties.SizeBegin = 0.5f;
	m_ParticleProperties.SizeVariation = 0.3f;
	m_ParticleProperties.SizeEnd = 0.0f;
	m_ParticleProperties.LifeTime = 3.0f;
	m_ParticleProperties.Velocity = { 0.0f, 0.0f };
	m_ParticleProperties.VelocityVariation = { 3.0f, 1.0f };
	m_ParticleProperties.Position = { 0.0f, 0.0f };
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

	static float angle = 0.0f;
	angle += glm::radians(45.0f) * timestep;

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Engine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_WhiteTexture, 1.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
	Engine::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_WhiteTexture, 1.0f, { 0.2f, 0.3f, 0.8f, 1.0f });
	Engine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerBoardTexture, 10.0f);
	Engine::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, angle, m_CheckerBoardTexture, 20.0f);
	Engine::Renderer2D::EndScene();

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 color = { (x + 5.0f) * 0.1f, 0.4f, (y + 5.0f) * 0.1f, 0.5f };
			Engine::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, m_WhiteTexture, 1.0f, color);
		}
	}
	Engine::Renderer2D::EndScene();

	if (Engine::Input::IsMouseButtonPressed(NG_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Engine::Input::GetMousePosition();
		auto width = Engine::Application::Get().GetWindow().GetWidth();
		auto height = Engine::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetCamera().GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_ParticleProperties.Position = { x + pos.x, y + pos.y };

		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_ParticleProperties);
	}
	m_ParticleSystem.OnUpdate(timestep);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	
	auto stats = Engine::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
}
