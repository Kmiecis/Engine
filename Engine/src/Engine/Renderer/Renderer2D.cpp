#include "Renderer2D.h"

#include "Engine/Core/Core.h"
#include "Engine/Platform/OpenGL/OpenGLShader.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			+0.5f, -0.5f, 0.0f,
			+0.5f, +0.5f, 0.0f,
			-0.5f, +0.5f, 0.0f
		};

		Ref<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		Ref<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}
	
	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}
	
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	
	void Renderer2D::EndScene()
	{
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);
		
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		glm::mat4 transform = translate * scale;

		s_Data->FlatColorShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}