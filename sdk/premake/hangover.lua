-- ------------------------------------------
-- Hangover Engine: shared Premake settings for game projects.
-- Exported to $HANGOVER_BIN/premake and included by every project's premake5.lua,
-- so engine-side changes reach all projects without touching them.
-- ------------------------------------------

hangover = hangover or {}

local bin = os.getenv("HANGOVER_BIN")
if not bin then
	error("HANGOVER_BIN is not set. Launch liquor or the Hangover editor once to register the engine.", 0)
end

hangover.bin = path.normalize(bin)

-- engine.lua is written by the engine build and records which configuration this bin holds
include(path.join(hangover.bin, "premake/engine.lua"))

-- Solution-wide settings. Call inside workspace "...".
function hangover.workspace()
	-- Only the configuration the engine was built in: game code must match the running editor
	configurations { hangover.config }
	architecture "x86_64"

	-- Generated IDE files and build output stay out of the project's own folders
	location(path.join(_MAIN_SCRIPT_DIR, "build"))

	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"

	filter "configurations:Release"
		runtime "Release"
		symbols "Off"
		optimize "On"
		defines { "NDEBUG" }

	filter {}
end

-- The game DLL the editor loads. Call inside project "game".
function hangover.gamemodule()
	kind "SharedLib"

	targetdir(path.join(_MAIN_SCRIPT_DIR, "build/bin/%{cfg.buildcfg}"))
	objdir(path.join(_MAIN_SCRIPT_DIR, "build/obj/%{cfg.buildcfg}"))

	-- {PROJECT_NAME}.generated.h exports the game API while this is defined
	defines { "game_EXPORTS" }

	includedirs { path.join(hangover.bin, "include") }
	libdirs { hangover.bin }
	links { "hangover-core", "EASTL" }

	-- Engine-owned boilerplate compiled into every game module
	files { path.join(hangover.bin, "source/GameModule.cpp") }
	vpaths { ["Engine"] = path.join(hangover.bin, "source/**") }
end
