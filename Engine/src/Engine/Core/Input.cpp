#include "Input.h"

#ifdef NG_PLATFORM_WINDOWS
#include "Engine/Platform/Windows/WindowsInput.h"
#endif

namespace Engine
{
	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef NG_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
#else
		NG_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
} 