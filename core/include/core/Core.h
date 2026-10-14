#pragma once

// core_EXPORTS is defined by CMake only while building the core DLL itself;
// CORE_STATIC by core-static, the copy linked straight into liquor
#if defined(CORE_STATIC)
	#define CORE_API
#elif defined(_WIN32)
	#if defined(core_EXPORTS)
		#define CORE_API __declspec(dllexport)
	#else
		#define CORE_API __declspec(dllimport)
	#endif
#else
	#define CORE_API __attribute__((visibility("default")))
#endif
