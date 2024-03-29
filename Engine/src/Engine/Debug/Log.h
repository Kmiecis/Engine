#pragma once

#include "Engine/Core/Base.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Engine
{
    class Log
    {
    public:
        static void Init();

        inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define NG_CORE_TRACE(...)    ::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NG_CORE_INFO(...)     ::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NG_CORE_WARN(...)     ::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NG_CORE_ERROR(...)    ::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NG_CORE_CRITICAL(...) ::Engine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define NG_TRACE(...)         ::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NG_INFO(...)          ::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
#define NG_WARN(...)          ::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NG_ERROR(...)         ::Engine::Log::GetClientLogger()->error(__VA_ARGS__)
#define NG_CRITICAL(...)      ::Engine::Log::GetClientLogger()->critical(__VA_ARGS__)