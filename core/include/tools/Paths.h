#pragma once

#include "core/Core.h"

#include <filesystem>

// Folder containing the running executable
CORE_API std::filesystem::path GetExecutableDir();

// resources/ is copied next to the executables by the build, see CMakeLists.txt
CORE_API std::filesystem::path GetResourcesDir();
