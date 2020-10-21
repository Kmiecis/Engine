#include "GraphicsContext.h"

#include "Engine/Platform/OpenGL/OpenGLContext.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    NG_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		NG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}
}