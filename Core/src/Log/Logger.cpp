#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
#include <Windows.h>
#endif

#include "Engine/Core/Log/Logger.h"

#include <iostream>
#include <chrono>
#include <cstdarg>
#include <cstdio>

namespace Engine::Core 
{
    static const char* GetFontColorCodeRaw(Log::Logger::LogLevel logLevel) 
    {
        using L = Log::Logger::LogLevel;
        switch (logLevel) 
        {
            case L::ASSERT:   return "\033[38;5;208m";
            case L::TRACE:    return "\033[90m";
            case L::DBUG:     return "\033[32m";
            case L::INFO:     return "\033[38;5;12m";
            case L::WARNING:  return "\033[38;5;220m";
            case L::ERR:      return "\033[38;5;208m";
            case L::CRITICAL: return "\033[31m";
            default:          return "\033[0m";
        }
    }

    static const char* GetLabelRaw(Log::Logger::LogLevel logLevel) 
    {
        using L = Log::Logger::LogLevel;
        switch (logLevel) 
        {
            case L::ASSERT:   return "[ASSERT]";
            case L::TRACE:    return "[TRACE]";
            case L::DBUG:     return "[DEBUG]";
            case L::INFO:     return "[INFO]";
            case L::WARNING:  return "[WARNING]";
            case L::ERR:      return "[ERROR]";
            case L::CRITICAL: return "[CRITICAL]";
            default:          return "[LOG]";
        }
    }

    void Log::Logger::LogMessage(Log::Logger::LogLevel logLevel, const char* format, ...) 
    {
        va_list args;
        va_start(args, format);

        va_list args_copy;
        va_copy(args_copy, args);
        int size = vsnprintf(nullptr, 0, format, args_copy);
        va_end(args_copy);

        if (size <= 0) { va_end(args); return; }

        std::vector<char> dynamicBuffer(size + 1);
        vsnprintf(dynamicBuffer.data(), dynamicBuffer.size(), format, args);
        va_end(args);

        char timeBuffer[16];
        auto time = std::chrono::system_clock::now();
        
        std::time_t now_time = std::chrono::system_clock::to_time_t(time);
        std::tm local_tm;

        localtime_s(&local_tm, &now_time);
        strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &local_tm);

        const char* color = GetFontColorCodeRaw(logLevel);
        const char* label = GetLabelRaw(logLevel);

        printf("%s[%s] %s: %s\033[0m\n", color, timeBuffer, label, dynamicBuffer.data());
        fflush(stdout);

        #ifdef _WIN32
            OutputDebugStringA(dynamicBuffer.data());
            OutputDebugStringA("\n");
        #endif
    }
}