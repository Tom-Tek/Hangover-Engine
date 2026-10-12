#include "Paths.h"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <windows.h>
#endif

#include <string>

namespace fs = std::filesystem;

fs::path GetExecutableDir()
{
#ifdef _WIN32
	// GetModuleFileNameW truncates silently, so grow the buffer until the whole path fits
	std::wstring buffer(MAX_PATH, L'\0');
	while (true)
	{
		const DWORD length = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
		if (length == 0)
			return {};

		if (length < buffer.size())
		{
			buffer.resize(length);
			break;
		}

		buffer.resize(buffer.size() * 2);
	}

	return fs::path(buffer).parent_path();
#else
	std::error_code ec;
	return fs::canonical("/proc/self/exe", ec).parent_path();
#endif
}

fs::path GetResourcesDir()
{
	return GetExecutableDir() / "resources";
}
