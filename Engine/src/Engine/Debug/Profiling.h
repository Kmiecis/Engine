#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>

#include "Engine/Debug/Log.h"

namespace Engine
{
    struct ProfileResult
    {
        std::string Name;

        std::chrono::duration<double, std::micro> Start;
        std::chrono::microseconds ElapsedTime;
        std::thread::id ThreadID;
    };

    struct ProfilingSession
    {
        std::string Name;
    };

    class Profiler
    {
    public:
        Profiler(const Profiler&) = delete;
        Profiler(Profiler&&) = delete;

        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession)
            {
                // If there is already a current session, then close it before beginning new one.
                // Subsequent profiling output meant for the original session will end up in the
                // newly opened session instead.  That's better than having badly formatted
                // profiling output.
                if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
                {
                    NG_CORE_ERROR("Profiler::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
                }
                InternalEndSession();
            }
            m_OutputStream.open(filepath);

            if (m_OutputStream.is_open())
            {
                m_CurrentSession = new ProfilingSession({ name });
                WriteHeader();
            }
            else
            {
                if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
                {
                    NG_CORE_ERROR("Profiler could not open results file '{0}'.", filepath);
                }
            }
        }

        void EndSession()
        {
            std::lock_guard lock(m_Mutex);
            InternalEndSession();
        }

        void WriteProfile(const ProfileResult& result)
        {
            std::stringstream json;

            json << std::setprecision(3) << std::fixed;
            json << ",{";
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
            json << "\"name\":\"" << result.Name << "\",";
            json << "\"ph\":\"X\",";
            json << "\"pid\":0,";
            json << "\"tid\":" << result.ThreadID << ",";
            json << "\"ts\":" << result.Start.count();
            json << "}";

            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession)
            {
                m_OutputStream << json.str();
                m_OutputStream.flush();
            }
        }

        static Profiler& Get()
        {
            static Profiler instance;
            return instance;
        }
    private:
        Profiler()
            : m_CurrentSession(nullptr)
        {
        }

        ~Profiler()
        {
            EndSession();
        }

        void WriteHeader()
        {
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
            m_OutputStream.flush();
        }

        void WriteFooter()
        {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        // Note: you must already own lock on m_Mutex before
        // calling InternalEndSession()
        void InternalEndSession()
        {
            if (m_CurrentSession)
            {
                WriteFooter();
                m_OutputStream.close();
                delete m_CurrentSession;
                m_CurrentSession = nullptr;
            }
        }

    private:
        std::mutex m_Mutex;
        ProfilingSession* m_CurrentSession;
        std::ofstream m_OutputStream;
    };

    class ProfilingTimer
    {
    public:
        ProfilingTimer(const char* name)
            : m_Name(name), m_Stopped(false)
        {
            m_StartTimepoint = std::chrono::steady_clock::now();
        }

        ~ProfilingTimer()
        {
            if (!m_Stopped)
            {
                Stop();

                m_Stopped = true;
            }
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::steady_clock::now();
            auto highResStart = std::chrono::duration<double, std::micro>(m_StartTimepoint.time_since_epoch());
            auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

            Profiler::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });
        }

    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
        bool m_Stopped;
    };

    namespace ProfilerUtils
    {
        template <size_t N>
        struct ChangeResult
        {
            char Data[N];
        };

        template <size_t N, size_t K>
        constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
        {
            ChangeResult<N> result = {};

            size_t srcIndex = 0;
            size_t dstIndex = 0;
            while (srcIndex < N)
            {
                size_t matchIndex = 0;
                while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
                    matchIndex++;
                if (matchIndex == K - 1)
                    srcIndex += matchIndex;
                result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
                srcIndex++;
            }
            return result;
        }
    }
}

// The result should be uploaded to 'chrome://tracing'
#define PROFILING_ENABLED 0
#if PROFILING_ENABLED
    // Resolve which function signature macro will be used. Note that this only
    // is resolved when the (pre)compiler starts, so the syntax highlighting
    // could mark the wrong one in your editor!
    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
        #define PROFILE_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__DMC__) && (__DMC__ >= 0x810)
        #define PROFILE_FUNC_SIG __PRETTY_FUNCTION__
    #elif (defined(__FUNCSIG__) || (_MSC_VER))
        #define PROFILE_FUNC_SIG __FUNCSIG__
    #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
        #define PROFILE_FUNC_SIG __FUNCTION__
    #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
        #define PROFILE_FUNC_SIG __FUNC__
    #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
        #define PROFILE_FUNC_SIG __func__
    #elif defined(__cplusplus) && (__cplusplus >= 201103)
        #define PROFILE_FUNC_SIG __func__
    #else
        #define PROFILE_FUNC_SIG "PROFILE_FUNC_SIG unknown!"
    #endif

    #define PROFILE_BEGIN_SESSION(name, filepath) ::Engine::Profiler::Get().BeginSession(name, filepath)
    #define PROFILE_END_SESSION() ::Engine::Profiler::Get().EndSession()
    #define PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::Engine::ProfilerUtils::CleanupOutputString(name, "__cdecl ");\
                                                   ::Engine::ProfilingTimer timer##line(fixedName##line.Data)
    #define PROFILE_SCOPE_LINE(name, line) PROFILE_SCOPE_LINE2(name, line)
    #define PROFILE_SCOPE(name) PROFILE_SCOPE_LINE(name, __LINE__)
    #define PROFILE_FUNCTION() PROFILE_SCOPE(PROFILE_FUNC_SIG)
#else
    #define PROFILE_BEGIN_SESSION(name, filepath)
    #define PROFILE_END_SESSION()
    #define PROFILE_SCOPE(name)
    #define PROFILE_FUNCTION()
#endif