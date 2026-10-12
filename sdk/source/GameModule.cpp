// ------------------------------------------
// Hangover Engine: compiled into every game module by hangover.lua.
// Engine-owned; edits here are overwritten on the next engine build.
// ------------------------------------------

#include <cstddef>

// EASTL's default allocator calls these; every module that uses EASTL containers needs them
void* operator new[](size_t size, const char* /*name*/, int /*flags*/, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/)
{
	return ::operator new[](size);
}

// EASTL frees with a plain delete[], so this must not use aligned new (MSVC frees that memory
// differently). Like EASTL's reference implementation it relies on the default alignment
// (16 bytes on x64); containers needing more must be given an aligned allocator.
void* operator new[](size_t size, size_t /*alignment*/, size_t /*alignmentOffset*/, const char* /*name*/, int /*flags*/,
	unsigned /*debugFlags*/, const char* /*file*/, int /*line*/)
{
	return ::operator new[](size);
}
