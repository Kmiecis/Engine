#include "Buffer.h"

#include "Renderer.h"
#include "Engine/Log.h"
#include "Engine/Platform/OpenGL/OpenGLBuffer.h"

namespace Engine
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
				LOG_CORE_ASSERT(false, "RendererAPI::None is not supported.");
				return nullptr;

			case RendererAPI::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
		}

		LOG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}
	
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
				LOG_CORE_ASSERT(false, "RendererAPI::None is not supported.");
				return nullptr;

			case RendererAPI::OpenGL:
				return new OpenGLIndexBuffer(indices, count);
		}

		LOG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}
}
