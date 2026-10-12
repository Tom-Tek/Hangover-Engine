-- {PROJECT_NAME}: describes this game's code to Premake.
-- Run GenerateProjectFiles after adding or removing source files; the IDE files go in build/.

local bin = os.getenv("HANGOVER_BIN")
if not bin then
	error("HANGOVER_BIN is not set. Launch liquor or the Hangover editor once to register the engine.", 0)
end
include(path.join(bin, "premake/hangover.lua"))

workspace "{PROJECT_NAME}"
	hangover.workspace()
	startproject "game"

project "game"
	hangover.gamemodule()

	files { "source/**.h", "source/**.cpp" }
	includedirs { "source" }
