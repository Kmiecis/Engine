#include "Buffer.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/Log.h"
#include "Engine/Platform/OpenGL/OpenGLBuffer.h"

namespace Engine
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				LOG_CORE_ASSERT(false, "RendererAPI::None is not supported.");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
		}

		LOG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}
	
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				LOG_CORE_ASSERT(false, "RendererAPI::None is not supported.");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return new OpenGLIndexBuffer(indices, count);
		}

		LOG_CORE_ASSERT(false, "RendererAPI::Unknown");
		return nullptr;
	}
}
