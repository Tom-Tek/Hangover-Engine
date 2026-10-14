#pragma once

#include "core/Core.h"

#include <filesystem>

// Folder containing the running executable
CORE_API std::filesystem::path GetExecutableDir();

// resources/ is copied next to the executables by the build, see CMakeLists.txt
CORE_API std::filesystem::path GetResourcesDir();

// TODO!
// Move other paths functions into this namespace for sake of organization
namespace paths
{
	// True if path is dir itself or anywhere inside it; resolves "..", symlinks and letter case first
	CORE_API bool IsInsideDirectory(const std::filesystem::path& path, const std::filesystem::path& dir);
}