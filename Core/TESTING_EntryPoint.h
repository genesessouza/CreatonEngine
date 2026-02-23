#pragma once

#include <Engine/Core/RendererFactory.h>
#include <Engine/Core/RenderCommand.h>

#include <Engine/Core/Application.h>
#include <Engine/Core/MemoryManager.h>

#include <Windows.h>

Engine::Core::Application* CreateApplication();

int main(int argc, char** argv)
{
	// ------- ENABLE LOG COLORS ON EXE CONSOLE --------
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut == INVALID_HANDLE_VALUE) return -1;

		DWORD dwMode = 0;
		if (!GetConsoleMode(hOut, &dwMode)) return -1;

		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);
	}

	/* ----------------------------------------------------------------
	*  ----------------------------------------------------------------
	*  ----------------------- STARTING POINT -------------------------
	*  ----------------------------------------------------------------
	*  ----------------------------------------------------------------
	*/
	{
		auto app = CreateApplication();
		if (!app)
		{
			CRTN_LOG_CRITICAL("<EntryPoint::Main>: Application could not be created!");
			CRTN_ASSERT(!app, "<EntryPoint::Main>: Application has crashed!");

			return -1;
		}

		app->Run();
		if (false)
		{
			CRTN_LOG_CRITICAL("<EntryPoint::Main>: Application didn't run!");
			CRTN_ASSERT(false, "<EntryPoint::Main>: Failed to run application!");

			return -1;
		}

		if (!app)
		{
			CRTN_LOG_CRITICAL("<EntryPoint::Main>: Tried to delete nullpointer Application!");
			CRTN_ASSERT(!app, "<EntryPoint::Main>: Application was already deleted!");

			return -1;
		}

		delete app;
		if (false)
		{
			CRTN_LOG_CRITICAL("<EntryPoint::Main>: Application deletion has failed!");
			CRTN_ASSERT(!app, "<EntryPoint::Main>: Could not delete application!");

			return -1;
		}
	}

	// MEMORY TRACKING
	Engine::Core::Memory::MemoryTracker::DumpLeaks();
	return 0;
}