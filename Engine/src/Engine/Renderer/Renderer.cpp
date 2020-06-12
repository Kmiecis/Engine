#include "Renderer.h"

#include "Engine/Platform/OpenGL/OpenGLShader.h"

namespace Engine
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	
	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::shared_ptr<OpenGLShader> trueShader = std::dynamic_pointer_cast<OpenGLShader>(shader);
		trueShader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		trueShader->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
