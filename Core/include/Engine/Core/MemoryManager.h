#pragma once

#include <map>
#include <iostream>
#include <mutex>

namespace Engine::Core::Memory
{
    struct AllocationInfo 
    {
        void* Address = nullptr;
        size_t Size = 0;
        const char* File = nullptr;
        int Line = 0;
    };

    class MemoryTracker 
    {
    public:
        static void RecordAllocation(void* address, size_t size, const char* file, int line);
        static void RecordDeallocation(void* address);
        static void DumpLeaks();
    private:
        static constexpr int MAX_ALLOCATIONS = 10000;
        static AllocationInfo s_Allocations[MAX_ALLOCATIONS];
        static size_t s_TotalAllocated;
        static size_t s_TotalFreed;
        static std::mutex s_Mutex;
    };
}

void* operator new(size_t size, const char* file, int line);
void operator delete(void* address) noexcept;
void operator delete(void* address, const char* file, int line) noexcept;

#define new new(__FILE__, __LINE__)