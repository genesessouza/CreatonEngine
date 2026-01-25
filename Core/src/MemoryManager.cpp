#include "Engine/Core/MemoryManager.h"

#include <cstdio>

#undef new 

namespace Engine::Core::Memory
{
	AllocationInfo MemoryTracker::s_Allocations[MAX_ALLOCATIONS] = {};
	size_t MemoryTracker::s_TotalAllocated = 0;
	size_t MemoryTracker::s_TotalFreed = 0;
	std::mutex MemoryTracker::s_Mutex;
	static bool s_IsTrackerActive = true;

	void MemoryTracker::RecordAllocation(void* address, size_t size, const char* file, int line) {
		if (!s_IsTrackerActive || !address) return;

		std::lock_guard<std::mutex> lock(s_Mutex);
		s_TotalAllocated += size;

		for (int i = 0; i < MAX_ALLOCATIONS; i++) {
			if (s_Allocations[i].Address == nullptr) {
				s_Allocations[i] = { address, size, file, line };
				return;
			}
		}
	}

	void MemoryTracker::RecordDeallocation(void* address) {
		if (!address) return;

		std::lock_guard<std::mutex> lock(s_Mutex);
		for (int i = 0; i < MAX_ALLOCATIONS; i++) {
			if (s_Allocations[i].Address == address) {
				s_TotalFreed += s_Allocations[i].Size;
				s_Allocations[i].Address = nullptr; // Libera o slot
				return;
			}
		}
	}

	void MemoryTracker::DumpLeaks() 
	{
		s_IsTrackerActive = false;

		bool leaked = false;
		for (int i = 0; i < MAX_ALLOCATIONS; i++) {
			if (s_Allocations[i].Address != nullptr) {
				printf("\033[38;5;220m[MEMORY MANAGER]: Leak detected on address: %p | %zu bytes | %s:%d\033[0m\n",
					s_Allocations[i].Address, s_Allocations[i].Size,
					s_Allocations[i].File, s_Allocations[i].Line);
				leaked = true;
			}
		}

		if (!leaked)
			printf("\033[38;5;14m\n[MEMORY MANAGER]: Zero memory leaks detected.\033[0m\n");

		printf("\033[38;5;14m[MEMORY MANAGER]: Total Allocated: %zu | Total Freed: %zu\033[0m\n", s_TotalAllocated, s_TotalFreed);
	}
}

void* operator new(size_t size, const char* file, int line)
{
	void* address = malloc(size);
	if (address)
		Engine::Core::Memory::MemoryTracker::RecordAllocation(address, size, file, line);

	return address;
}

void operator delete(void* address) noexcept
{
	Engine::Core::Memory::MemoryTracker::RecordDeallocation(address);
	free(address);
}

void operator delete(void* address, const char* file, int line) noexcept
{
	Engine::Core::Memory::MemoryTracker::RecordDeallocation(address);
	free(address);
}