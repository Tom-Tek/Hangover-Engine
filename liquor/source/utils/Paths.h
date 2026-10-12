#pragma once

#include <filesystem>

// Folder containing the running executable
std::filesystem::path GetExecutableDir();

// resources/ is copied next to the executables by the build, see CMakeLists.txt
std::filesystem::path GetResourcesDir();
