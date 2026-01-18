#include "Engine/Core/Log/Logger.h"

#include <chrono>
#include <cstdarg>
#include <iostream>

namespace Engine::Core
{
	static std::string GetFontColorCode(Log::Logger::LogLevel logLevel)
	{
		switch (logLevel) {
		case Log::Logger::LogLevel::ASSERT:   return "\033[38;5;208m";  // Orange
		case Log::Logger::LogLevel::TRACE:    return "\033[90m"; // Grey
		case Log::Logger::LogLevel::DBUG:     return "\033[32m"; // Green
		case Log::Logger::LogLevel::INFO:     return "\033[36m"; // Cyan
		case Log::Logger::LogLevel::WARNING:  return "\033[33m"; // Yellow
		case Log::Logger::LogLevel::ERR:      return "\033[38;5;208m"; // Orange
		case Log::Logger::LogLevel::CRITICAL: return "\033[31m"; // Red
		default:                 return "\033[0m";  // White
		}
	}

	static std::string GetCurrentTimeString()
	{
		auto time = std::chrono::system_clock::now();
		std::time_t now_time = std::chrono::system_clock::to_time_t(time);
		std::tm local_tm;
		localtime_s(&local_tm, &now_time);

		std::ostringstream oss;
		oss << std::put_time(&local_tm, "%H:%M:%S");
		return oss.str();
	}

	void Log::Logger::LogMessage(Log::Logger::LogLevel logLevel, const char* format, ...)
	{
		char buffer[1024];
		va_list args;
		va_start(args, format);
		vsnprintf(buffer, sizeof(buffer), format, args);
		va_end(args);

		std::string timeStr = GetCurrentTimeString();
		std::string color = GetFontColorCode(logLevel);

		const char* label = "";
		switch (logLevel) {
		case LogLevel::ASSERT:   label = "[ASSERT]"; break;
		case LogLevel::TRACE:    label = "[TRACE]"; break;
		case LogLevel::DBUG:     label = "[DEBUG]"; break;
		case LogLevel::INFO:     label = "[INFO]"; break;
		case LogLevel::WARNING:  label = "[WARNING]"; break;
		case LogLevel::ERR:      label = "[ERROR]"; break;
		case LogLevel::CRITICAL: label = "[CRITICAL]"; break;
		}

		std::cout << color << "[" << timeStr << "] " << label << ": " << buffer << "\033[0m\n";
	}
}