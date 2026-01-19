#pragma once

#include <assert.h>

namespace Engine::Core::Log
{
	class Logger
	{
	public:
		enum class LogLevel
		{
			ASSERT,
			TRACE,
			DBUG,
			INFO,
			WARNING,
			ERR,
			CRITICAL
		};
	public:
		static void LogMessage(LogLevel logLevel, const char* format, ...);
	};
}

//////////////////////////////////////
//////////// LOG MACROS //////////////
//////////////////////////////////////

#define CRTN_CHECK_PTR(ptr) \
    do { \
        if (!(ptr)) { \
            CRTN_LOG_CRITICAL("Null Pointer detected: Pointer: %s | File: %s | Line: %d", #ptr, __FILE__, __LINE__); \
            __debugbreak(); \
        } \
    } while (0)

#define CRTN_ASSERT(condition, ...) \
    do { \
        if (!(condition)) { \
            Engine::Core::Log::Logger::LogMessage(Engine::Core::Log::Logger::LogLevel::CRITICAL, \
                "Assert result: %s. File: %s, Line: %d", #condition, __FILE__, __LINE__); \
            \
            Engine::Core::Log::Logger::LogMessage(Engine::Core::Log::Logger::LogLevel::CRITICAL, __VA_ARGS__); \
            \
            __debugbreak(); \
        } \
    } while (0)

#define CRTN_LOG_TRACE(...) \
		Engine::Core::Log::Logger::LogMessage(Engine::Core::Log::Logger::LogLevel::TRACE, __VA_ARGS__)

#define CRTN_LOG_DEBUG(...) \
		Engine::Core::Log::Logger::LogMessage(Engine::Core::Log::Logger::LogLevel::DBUG, __VA_ARGS__)

#define CRTN_LOG_INFO(...) \
		Engine::Core::Log::Logger::LogMessage(Engine::Core::Log::Logger::LogLevel::INFO, __VA_ARGS__)

#define CRTN_LOG_WARNING(...) \
		Engine::Core::Log::Logger::LogMessage(Engine::Core::Log::Logger::LogLevel::WARNING, __VA_ARGS__)

#define CRTN_LOG_ERROR(...) \
		Engine::Core::Log::Logger::LogMessage(Engine::Core::Log::Logger::LogLevel::ERR, __VA_ARGS__)

#define CRTN_LOG_CRITICAL(...) \
		Engine::Core::Log::Logger::LogMessage(Engine::Core::Log::Logger::LogLevel::CRITICAL, __VA_ARGS__)