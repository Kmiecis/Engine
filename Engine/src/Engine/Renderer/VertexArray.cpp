#include "VertexArray.h"

#include "Engine/Platform/OpenGL/OpenGLVertexArray.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    NG_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
        }

        NG_CORE_ASSERT(false, "RendererAPI::Unknown");
        return nullptr;
    }
}