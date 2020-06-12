#include <Engine.h>

#include "Engine/Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_VertexArray.reset(Engine::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f,
			+0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f,
			+0.0f, +0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f,
		};

		std::shared_ptr<Engine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Engine::VertexBuffer::Create(vertices, sizeof(vertices)));
		Engine::BufferLayout layout = {
			{ Engine::ShaderDataType::Float3, "a_Position" },
			{ Engine::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = {
			0, 1, 2
		};

		std::shared_ptr<Engine::IndexBuffer> indexBuffer;
		indexBuffer.reset(Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVertexArray.reset(Engine::VertexArray::Create());

		float squareVertices[3 * 7] = {
			-0.75f, -0.75f, 0.0f,
			+0.75f, -0.75f, 0.0f,
			+0.75f, +0.75f, 0.0f,
			-0.75f, +0.75f, 0.0f
		};

		std::shared_ptr<Engine::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Engine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVertexBuffer->SetLayout({
			{ Engine::ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<Engine::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Engine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		std::string vertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1);
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 a_Color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				a_Color = vec4(v_Position * 0.5 + 0.5, 1.0);
				a_Color = v_Color;
			}
		)";

		m_Shader.reset(Engine::Shader::Create(vertexSource, fragmentSource));

		std::string flatColorShaderVertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1);
			}
		)";

		std::string flatColorShaderFragmentSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 a_Color;

			in vec3 v_Position;

			uniform vec3 u_Color;
			
			void main()
			{
				a_Color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader.reset(Engine::Shader::Create(flatColorShaderVertexSource, flatColorShaderFragmentSource));
	}

	void OnUpdate(Engine::Timestep timestep) override
	{
		auto cameraPosition = m_Camera.GetPosition();

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_LEFT))
			cameraPosition.x -= m_CameraMoveSpeed * timestep;

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_RIGHT))
			cameraPosition.x += m_CameraMoveSpeed * timestep;

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_DOWN))
			cameraPosition.y -= m_CameraMoveSpeed * timestep;

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_UP))
			cameraPosition.y += m_CameraMoveSpeed * timestep;

		m_Camera.SetPosition(cameraPosition);


		auto cameraRotation = m_Camera.GetRotation();

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_D))
			cameraRotation += m_CameraRotationSpeed * timestep;

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_A))
			cameraRotation -= m_CameraRotationSpeed * timestep;

		m_Camera.SetRotation(cameraRotation);

		Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Engine::RenderCommand::Clear();

		Engine::Renderer::BeginScene(m_Camera);
		
		std::shared_ptr<Engine::OpenGLShader> trueFlatColorShader = std::dynamic_pointer_cast<Engine::OpenGLShader>(m_FlatColorShader);
		trueFlatColorShader->Bind();
		trueFlatColorShader->UploadUniformFloat3("u_Color", m_SquareColor);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int x = 0; x < 5; ++x)
		{
			for (int y = 0; y < 5; ++y)
			{
				glm::vec3 pos(x * 0.21f, y * 0.21f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);
				Engine::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform * scale);
			}
		}
		Engine::Renderer::Submit(m_Shader, m_VertexArray);

		Engine::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Engine::Event& event) override
	{
	}

private:
	std::shared_ptr<Engine::Shader> m_Shader;
	std::shared_ptr<Engine::VertexArray> m_VertexArray;

	std::shared_ptr<Engine::Shader> m_FlatColorShader;
	std::shared_ptr<Engine::VertexArray> m_SquareVertexArray;

	Engine::OrthographicCamera m_Camera;
	float m_CameraMoveSpeed = 10.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = glm::vec3(0.2f, 0.3f, 0.8f);
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
