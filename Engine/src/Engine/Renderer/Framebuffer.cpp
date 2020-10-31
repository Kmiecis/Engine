#include "Framebuffer.h"

#include "Engine/Platform/OpenGL/OpenGLFramebuffer.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferProperties& properties)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    NG_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(properties);
		}

		NG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}
}