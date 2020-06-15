#include "VertexArray.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				LOG_CORE_ASSERT(false, "RendererAPI::None is not supported.");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return new OpenGLVertexArray();
		}

		LOG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}
}
