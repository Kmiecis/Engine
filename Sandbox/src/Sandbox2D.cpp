#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVertexArray = Engine::VertexArray::Create();

	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		+0.5f, -0.5f, 0.0f,
		+0.5f, +0.5f, 0.0f,
		-0.5f, +0.5f, 0.0f
	};

	Engine::Ref<Engine::VertexBuffer> squareVertexBuffer;
	squareVertexBuffer = Engine::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVertexBuffer->SetLayout({
		{ Engine::ShaderDataType::Float3, "a_Position" }
	});
	m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

	uint32_t squareIndices[6] = {
		0, 1, 2, 2, 3, 0
	};

	Engine::Ref<Engine::IndexBuffer> squareIndexBuffer;
	squareIndexBuffer = Engine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

	m_FlatColorShader = Engine::Shader::Create("assets/shaders/FlatColor.glsl");
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

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	Engine::Ref<Engine::OpenGLShader> trueFlatColorShader = std::dynamic_pointer_cast<Engine::OpenGLShader>(m_FlatColorShader);
	trueFlatColorShader->Bind();
	trueFlatColorShader->UploadUniformFloat4("u_Color", m_SquareColor);

	Engine::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Engine::Renderer::EndScene();
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
