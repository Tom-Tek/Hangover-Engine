// ------------------------------------------
// Hangover Engine: engine-owned code every game module needs.
// Built once into bin/hangover-game.lib, which hangover.lua links into every game,
// so it never appears (or gets edited) inside game projects.
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
