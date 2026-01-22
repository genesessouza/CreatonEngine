#pragma once

#include <Engine/Core/Application.h>
#include <Engine/Core/MemoryManager.h>

#include <Windows.h>

Engine::Core::Application* CreateApplication();

//#ifdef NDEBUG
//constexpr auto RELEASE_BUILD = 1;
//#else
//	#ifdef _DEBUG
//constexpr auto DEBUG_BUILD = 0;
//	#endif	
//#endif

LONG WINAPI CrashHandler(EXCEPTION_POINTERS* ExceptionInfo) {
	const char* errorType = "Unknown Exception";

	switch (ExceptionInfo->ExceptionRecord->ExceptionCode) 
	{
		case EXCEPTION_ACCESS_VIOLATION: errorType = "ACCESS VIOLATION (Invalid Memory Read/Write)"; break;
		case EXCEPTION_STACK_OVERFLOW:   errorType = "STACK OVERFLOW"; break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO: errorType = "INTEGER DIVIDE BY ZERO"; break;
	}

	void* faultingAddress = ExceptionInfo->ExceptionRecord->ExceptionAddress;

	Engine::Core::Log::Logger::LogMessage(Engine::Core::Log::Logger::LogLevel::CRITICAL, "CRITICAL ENGINE CRASH: %s at address %p", errorType, faultingAddress);

	return EXCEPTION_CONTINUE_SEARCH;
}

int main(int argc, char** argv)
{
	// Enable Log colors on build exe
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) return -1;

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) return -1;

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);

	// Starting point
	auto app = CreateApplication();
	CRTN_ASSERT(app != nullptr, "Application creation failed!");

	app->Run();
	CRTN_ASSERT(true, "Application run failed!");

	CRTN_ASSERT(app != nullptr, "Application is null before deletion!");
	delete app;
	CRTN_ASSERT(true, "Application deletion failed!");

	//#if RELEASE_BUILD 
	//	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	//	system("pause");
	//#endif

	Engine::Core::Memory::MemoryTracker::DumpLeaks();
	return 0;
}