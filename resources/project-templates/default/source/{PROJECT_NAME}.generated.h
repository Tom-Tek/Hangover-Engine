// Do NOT Modify OR Delete! Unless you know what you're doing of course :)

// --------------------------------------------- \\
// Auto Generated File created by Hangover Engine
//		- Target Project: {PROJECT_NAME}
//		- Editor Version: v{EDITOR_VERSION}
//		- Engine Version: v{ENGINE_VERSION}
// --------------------------------------------- \\

#pragma once

// HANGOVER_PACKAGED is set by the packaging build: everything is compiled into one exe,
// and the player's main() starts the runtime. Otherwise the editor loads this project as game.dll,
// and CMake defines game_EXPORTS while building it.
#if defined(HANGOVER_PACKAGED)
	#define {PROJECT_NAME_UPPER}_API
#elif defined(_WIN32) && defined(game_EXPORTS)
	#define {PROJECT_NAME_UPPER}_API __declspec(dllexport)
#elif defined(_WIN32)
	#define {PROJECT_NAME_UPPER}_API __declspec(dllimport)
#else
	#define {PROJECT_NAME_UPPER}_API
#endif