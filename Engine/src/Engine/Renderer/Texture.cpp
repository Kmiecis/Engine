#include "Texture.h"

#include "Renderer.h"
#include "Engine/Platform/OpenGL/OpenGLTexture.h"

namespace Engine
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				LOG_CORE_ASSERT(false, "RendererAPI::None is not supported.");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLTexture2D>(path);
		}

		LOG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}
}
