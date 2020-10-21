#include "Texture.h"

#include "Engine/Platform/OpenGL/OpenGLTexture.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    NG_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(width, height);
		}

		NG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    NG_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
		}

		NG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}
}