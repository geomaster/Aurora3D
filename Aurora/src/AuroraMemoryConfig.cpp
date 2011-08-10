#include "AuroraMemoryConfig.h"
using namespace Aurora;

void Aurora::deallocate(void* Mem)
{
    Allocators::Default::deallocateMemory(Mem);
}

void Aurora::deallocate(void* Mem, void*)
{
    Allocators::Default::deallocateMemory(Mem);
}

void Aurora::deallocate(void* Mem, const char* File, int Line)
{
    Allocators::Default::deallocateMemory(Mem);
}

void Aurora::deallocateArray(void* Mem)
{
    Allocators::Default::deallocateMemory(Mem);
}

void Aurora::deallocateArray(void* Mem, const char*, int)
{
    Allocators::Default::deallocateMemory(Mem);
}