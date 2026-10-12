@echo off
rem Generates the Visual Studio solution in build\ using the engine's Premake

if not defined HANGOVER_BIN (
	echo HANGOVER_BIN is not set. Launch liquor or the Hangover editor once to register the engine.
	pause
	exit /b 1
)

"%HANGOVER_BIN%\premake\premake5.exe" --file="%~dp0premake5.lua" vs2026
if errorlevel 1 (
	pause
	exit /b 1
)
