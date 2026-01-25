#pragma once

#include <Engine/Core/Application.h>
#include <Engine/Core/MemoryManager.h>

#include <Windows.h>

Engine::Core::Application* CreateApplication();

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

	Engine::Core::Memory::MemoryTracker::DumpLeaks();
	return 0;
}