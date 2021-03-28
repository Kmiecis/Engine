#include "Window.h"

#ifdef NG_PLATFORM_WINDOWS
#include "Engine/Platform/Windows/WindowsWindow.h"
#endif

namespace Engine
{
    Scope<Window> Window::Create(const WindowProperties& properties)
    {
#ifdef NG_PLATFORM_WINDOWS
        return CreateScope<WindowsWindow>(properties);
#else
        NG_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
}