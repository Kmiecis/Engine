#pragma once

#include <memory>

#include "Engine/Core/PlatformDetection.h"

#ifdef NG_DEBUG
    #if defined(NG_PLATFORM_WINDOWS)
        #define NG_DEBUGBREAK() __debugbreak()
    #elif defined (NG_PLATFORM_LINUX)
        #include <signal.h>
        #define NG_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak"
    #endif
    #define NG_ENABLE_ASSERTS
#endif

#ifdef NG_ENABLE_ASSERTS
    #define NG_ASSERT(x, ...) { if(!(x)) { NG_ERROR("Assertion Failed: {0}", __VA_ARGS__); NG_DEBUGBREAK(); } }
    #define NG_CORE_ASSERT(x, ...) { if(!(x)) { NG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); NG_DEBUGBREAK(); } }
#else
    #define NG_ASSERT(x, ...)
    #define NG_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define NG_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Engine
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
