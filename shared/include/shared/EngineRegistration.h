#pragma once

#include <filesystem>

// Points the HANGOVER_BIN user environment variable (and the per-user engine config file)
// at this engine's bin folder, so game projects can find headers and libraries.
// Called on every liquor/editor launch; the most recently launched engine wins.
// Only writes when the value changed. Returns false if registration failed.
bool RegisterEngine();

// The bin folder this build was configured with (HANGOVER_BIN_DIR in CMake)
std::filesystem::path GetEngineBinDir();

// Per-user config file holding the registered bin folder; used where environment variables
// are unreliable (macOS apps launched from Finder do not see shell variables)
std::filesystem::path GetEngineConfigPath();
