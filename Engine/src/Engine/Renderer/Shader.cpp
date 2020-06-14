#include "Shader.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Platform/OpenGL/OpenGLShader.h"

namespace Engine
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				LOG_CORE_ASSERT(false, "RendererAPI::None is not supported.");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLShader>(filepath);
		}

		LOG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				LOG_CORE_ASSERT(false, "RendererAPI::None is not supported.");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
		}

		LOG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}
}
