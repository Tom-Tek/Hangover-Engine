#include "toolkit/EngineRegistration.h"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <windows.h>
	#include <shlobj.h>
#endif

#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

namespace
{
	constexpr const char* k_BinVariable = "HANGOVER_BIN";

#ifdef _WIN32
	constexpr const wchar_t* k_EnvironmentKey = L"Environment";

	std::wstring ReadUserVariable(const wchar_t* name)
	{
		DWORD size = 0;
		if (RegGetValueW(HKEY_CURRENT_USER, k_EnvironmentKey, name, RRF_RT_REG_SZ, nullptr, nullptr, &size) != ERROR_SUCCESS)
			return {};

		std::wstring value(size / sizeof(wchar_t), L'\0');
		if (RegGetValueW(HKEY_CURRENT_USER, k_EnvironmentKey, name, RRF_RT_REG_SZ, nullptr, value.data(), &size) != ERROR_SUCCESS)
			return {};

		// size includes the terminating null
		value.resize(size / sizeof(wchar_t) > 0 ? size / sizeof(wchar_t) - 1 : 0);
		return value;
	}

	bool WriteUserVariable(const wchar_t* name, const std::wstring& value)
	{
		const DWORD bytes = static_cast<DWORD>((value.size() + 1) * sizeof(wchar_t));
		if (RegSetKeyValueW(HKEY_CURRENT_USER, k_EnvironmentKey, name, REG_SZ, value.c_str(), bytes) != ERROR_SUCCESS)
			return false;

		// Tell running programs (Explorer, new terminals) to reload the environment.
		// The timeout and SMTO_ABORTIFHUNG stop one frozen window from stalling our startup.
		SendMessageTimeoutW(HWND_BROADCAST, WM_SETTINGCHANGE, 0, reinterpret_cast<LPARAM>(k_EnvironmentKey),
			SMTO_ABORTIFHUNG, 1000, nullptr);
		return true;
	}

	bool RegisterEnvironmentVariable(const fs::path& binDir)
	{
		const std::wstring name(k_BinVariable, k_BinVariable + std::char_traits<char>::length(k_BinVariable));
		const std::wstring value = binDir.wstring();

		if (ReadUserVariable(name.c_str()) == value)
			return true;

		return WriteUserVariable(name.c_str(), value);
	}
#endif

	bool RegisterConfigFile(const fs::path& binDir)
	{
		const fs::path configPath = GetEngineConfigPath();
		if (configPath.empty())
			return false;

		const std::string contents = "bin = " + binDir.generic_string() + "\n";

		// Skip the write when nothing changed
		{
			std::ifstream existing(configPath, std::ios::binary);
			if (existing && std::string(std::istreambuf_iterator<char>(existing), std::istreambuf_iterator<char>()) == contents)
				return true;
		}

		std::error_code ec;
		fs::create_directories(configPath.parent_path(), ec);
		if (ec)
			return false;

		std::ofstream file(configPath, std::ios::binary);
		file << contents;
		return static_cast<bool>(file);
	}
}

fs::path GetEngineBinDir()
{
	return fs::path(HANGOVER_BIN_DIR).lexically_normal();
}

fs::path GetEngineConfigPath()
{
#if defined(_WIN32)
	PWSTR appData = nullptr;
	const bool found = SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &appData));
	const fs::path path = found ? fs::path(appData) / "Hangover" / "engine.cfg" : fs::path();
	CoTaskMemFree(appData); // required even when the call fails
	return path;
#elif defined(__APPLE__)
	if (const char* home = std::getenv("HOME"))
		return fs::path(home) / "Library" / "Application Support" / "Hangover" / "engine.cfg";
	return {};
#else
	if (const char* configHome = std::getenv("XDG_CONFIG_HOME"); configHome && *configHome)
		return fs::path(configHome) / "hangover" / "engine.cfg";
	if (const char* home = std::getenv("HOME"))
		return fs::path(home) / ".config" / "hangover" / "engine.cfg";
	return {};
#endif
}

bool RegisterEngine()
{
	const fs::path binDir = GetEngineBinDir();

	// A build handed to another machine has no bin folder there; registering it would point
	// projects at nothing, so leave whatever is registered alone
	std::error_code ec;
	if (!fs::is_directory(binDir, ec))
		return false;

	bool registered = RegisterConfigFile(binDir);
#ifdef _WIN32
	registered = RegisterEnvironmentVariable(binDir) && registered;
#endif
	return registered;
}
