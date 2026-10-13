#include "toolkit/EngineRegistration.h"
#include "toolkit/Paths.h"

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

	// Reads a string value under HKEY_CURRENT_USER; valueName nullptr means the key's default value
	std::wstring ReadUserString(const std::wstring& subKey, const wchar_t* valueName)
	{
		DWORD size = 0;
		if (RegGetValueW(HKEY_CURRENT_USER, subKey.c_str(), valueName, RRF_RT_REG_SZ, nullptr, nullptr, &size) != ERROR_SUCCESS)
			return {};

		std::wstring value(size / sizeof(wchar_t), L'\0');
		if (RegGetValueW(HKEY_CURRENT_USER, subKey.c_str(), valueName, RRF_RT_REG_SZ, nullptr, value.data(), &size) != ERROR_SUCCESS)
			return {};

		// size includes the terminating null
		value.resize(size / sizeof(wchar_t) > 0 ? size / sizeof(wchar_t) - 1 : 0);
		return value;
	}

	bool WriteUserString(const std::wstring& subKey, const wchar_t* valueName, const std::wstring& value)
	{
		const DWORD bytes = static_cast<DWORD>((value.size() + 1) * sizeof(wchar_t));
		return RegSetKeyValueW(HKEY_CURRENT_USER, subKey.c_str(), valueName, REG_SZ, value.c_str(), bytes) == ERROR_SUCCESS;
	}

	bool RegisterEnvironmentVariable(const fs::path& binDir)
	{
		const std::wstring name(k_BinVariable, k_BinVariable + std::char_traits<char>::length(k_BinVariable));
		const std::wstring value = binDir.wstring();

		if (ReadUserString(k_EnvironmentKey, name.c_str()) == value)
			return true;

		if (!WriteUserString(k_EnvironmentKey, name.c_str(), value))
			return false;

		// Tell running programs (Explorer, new terminals) to reload the environment.
		// The timeout and SMTO_ABORTIFHUNG stop one frozen window from stalling our startup.
		SendMessageTimeoutW(HWND_BROADCAST, WM_SETTINGCHANGE, 0, reinterpret_cast<LPARAM>(k_EnvironmentKey),
			SMTO_ABORTIFHUNG, 1000, nullptr);
		return true;
	}

	// Double-clicking a .hang project opens it in the editor next to this executable.
	// Per-user (HKCU\Software\Classes), so no admin rights are needed.
	bool RegisterProjectFileType(const fs::path& editorPath)
	{
		const std::wstring progId = L"Hangover.Project";
		const std::wstring classes = L"Software\\Classes\\";
		const std::wstring editor = L"\"" + editorPath.wstring() + L"\"";
		const std::wstring command = editor + L" \"%1\"";

		if (ReadUserString(classes + L".hang", nullptr) == progId
			&& ReadUserString(classes + progId + L"\\shell\\open\\command", nullptr) == command)
			return true;

		const bool written =
			WriteUserString(classes + L".hang", nullptr, progId) &&
			WriteUserString(classes + progId, nullptr, L"Hangover Project") &&
			WriteUserString(classes + progId + L"\\DefaultIcon", nullptr, editor + L",0") &&
			WriteUserString(classes + progId + L"\\shell\\open\\command", nullptr, command);

		// Makes Explorer pick up the new icon and handler without a restart
		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);
		return written;
	}
#endif // _WIN32

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

	// liquor and the editor are built into the same folder, so either can register the editor
	const fs::path editorPath = GetExecutableDir() / "plastered.exe";
	if (fs::exists(editorPath, ec))
		registered = RegisterProjectFileType(editorPath) && registered;
#endif
	return registered;
}
