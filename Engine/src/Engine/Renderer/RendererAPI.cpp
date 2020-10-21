#include "RendererAPI.h"

#include "Engine/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Engine
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:    NG_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}

		NG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}
}