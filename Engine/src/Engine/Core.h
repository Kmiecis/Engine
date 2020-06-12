#pragma once

#include <memory>

#ifdef ENGINE_PLATFORM_WINDOWS
	#ifdef ENGINE_DYNAMIC_LINK
		#ifdef ENGINE_BUILD_DLL
			#define ENGINE_API __declspec(dllexport)
		#else
			#define ENGINE_API __declspec(dllimport)
		#endif
	#else
		#define ENGINE_API
	#endif
#else
	#error Define supported platform
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Engine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}
