#include "Shader.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Platform/OpenGL/OpenGLShader.h"

namespace Engine
{
	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				LOG_CORE_ASSERT(false, "RendererAPI::None is not supported.");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return new OpenGLShader(vertexSource, fragmentSource);
		}

		LOG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}
}
