#include "CreateCommand.h"

#include "core/Globals.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <system_error>

namespace fs = std::filesystem;

namespace
{
	// HANGOVER_RESOURCES_DIR is set by CMake to the repository's resources folder
	const fs::path k_ProjectTemplatesDir = fs::path(HANGOVER_RESOURCES_DIR) / "project-templates";

	// Used for both project and template names, so neither can escape its parent folder
	bool IsValidName(std::string_view name)
	{
		if (name.empty() || name == "." || name == "..")
			return false;

		for (const char c : name)
		{
			const bool valid =
				(c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				(c >= '0' && c <= '9') ||
				c == '_' || c == '-' || c == ' ';
			if (!valid)
				return false;
		}

		// Windows silently strips trailing spaces from folder names
		return name.back() != ' ';
	}

	void PrintAvailableTemplates()
	{
		std::cerr << "Available templates:";

		std::error_code ec;
		for (const fs::directory_entry& entry : fs::directory_iterator(k_ProjectTemplatesDir, ec))
		{
			if (entry.is_directory(ec))
				std::cerr << ' ' << entry.path().filename().string();
		}

		std::cerr << '\n';
	}
}

CreateCommand::CreateCommand()
	: enact::Command("create")
{
}

void CreateCommand::Execute(std::string name, std::string location, std::string templateName)
{
	if (!IsValidName(name))
	{
		std::cerr << "Invalid project name '" << name << "'. Use letters, digits, spaces, '_' or '-'.\n";
		return;
	}

	std::error_code ec;

	// Checked before anything is written, so a typo never leaves a half-made project behind
	const fs::path templateDir = k_ProjectTemplatesDir / templateName;
	if (!IsValidName(templateName) || !fs::is_directory(templateDir, ec))
	{
		std::cerr << "Unknown project template '" << templateName << "'.\n";
		PrintAvailableTemplates();
		return;
	}

	m_root = fs::absolute(fs::path(location) / name, ec);
	if (ec)
	{
		std::cerr << "Invalid location '" << location << "': " << ec.message() << '\n';
		return;
	}

	// Never write into an existing folder, so nothing can be overwritten
	if (fs::exists(m_root, ec))
	{
		std::cerr << "'" << m_root.string() << "' already exists.\n";
		return;
	}

	DefineVariables(name);

	if (!MakeDirectory({}) || !PopulateProject(name) || !CopyTemplate(templateDir))
	{
		// The folder did not exist before this command, so removing it only discards our own partial output
		fs::remove_all(m_root, ec);
		return;
	}

	std::cout << "Created project '" << name << "' at " << m_root.string() << '\n';
}

void CreateCommand::DefineVariables(const std::string& name)
{
	m_variables = {
		{ "PROJECT_NAME", name },
		{ "ENGINE_VERSION", FormatVersion(k_RuntimeVersion) },
	};
}

bool CreateCommand::PopulateProject(const std::string& name)
{
	return
		MakeDirectory("content") &&
		MakeFile(name + ".hangover",
			"[project]\n"
			"name = " + name + "\n"
			"engine = " + FormatVersion(k_RuntimeVersion) + "\n");
}

bool CreateCommand::CopyTemplate(const fs::path& templateDir)
{
	std::error_code ec;
	for (auto it = fs::recursive_directory_iterator(templateDir, ec); !ec && it != fs::recursive_directory_iterator(); it.increment(ec))
	{
		const fs::path relative = FormatTemplate(it->path().lexically_relative(templateDir).generic_string());

		if (it->is_directory(ec))
		{
			if (!MakeDirectory(relative))
				return false;
			continue;
		}

		if (!it->is_regular_file(ec))
			continue;

		// Anything PopulateProject already made takes priority over the template
		if (fs::exists(m_root / relative, ec) || ec)
			continue;

		std::ifstream source(it->path(), std::ios::binary);
		if (!source)
		{
			std::cerr << "Failed to read template file '" << it->path().string() << "'.\n";
			return false;
		}

		const std::string contents{ std::istreambuf_iterator<char>(source), std::istreambuf_iterator<char>() };

		// Binary files (images, etc.) are copied untouched so formatting can never corrupt them
		const bool isBinary = contents.find('\0') != std::string::npos;
		if (!MakeFile(relative, isBinary ? contents : FormatTemplate(contents)))
			return false;
	}

	if (ec)
	{
		std::cerr << "Failed to read template '" << templateDir.string() << "': " << ec.message() << '\n';
		return false;
	}

	return true;
}

std::string CreateCommand::FormatTemplate(std::string_view text) const
{
	std::string result;
	result.reserve(text.size());

	const auto isNameChar = [](char c)
	{
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
	};

	// Single pass, so a value that itself contains {...} is never expanded again
	size_t position = 0;
	while (true)
	{
		const size_t open = text.find('{', position);
		if (open == std::string_view::npos)
			break;

		result.append(text, position, open - position);

		// Names are [A-Za-z0-9_] only, so a stray '{' can never swallow text up to some later '}'
		size_t close = open + 1;
		while (close < text.size() && isNameChar(text[close]))
			++close;

		// ${...} belongs to CMake and shell scripts, never to us
		const bool isDollarVariable = open > 0 && text[open - 1] == '$';
		const bool closed = close < text.size() && text[close] == '}';
		const auto variable = closed && !isDollarVariable
			? m_variables.find(std::string(text.substr(open + 1, close - open - 1)))
			: m_variables.end();

		if (variable != m_variables.end())
		{
			result += variable->second;
			position = close + 1;
		}
		else
		{
			result += '{';
			position = open + 1;
		}
	}

	result.append(text, position);
	return result;
}

bool CreateCommand::MakeDirectory(const fs::path& path)
{
	const fs::path fullPath = m_root / path;

	std::error_code ec;
	fs::create_directories(fullPath, ec);
	if (ec)
	{
		std::cerr << "Failed to create folder '" << fullPath.string() << "': " << ec.message() << '\n';
		return false;
	}

	return true;
}

bool CreateCommand::MakeFile(const fs::path& path, std::string_view contents)
{
	if (path.has_parent_path() && !MakeDirectory(path.parent_path()))
		return false;

	const fs::path fullPath = m_root / path;

	std::ofstream file(fullPath, std::ios::binary);
	file << contents;
	if (!file)
	{
		std::cerr << "Failed to write file '" << fullPath.string() << "'.\n";
		return false;
	}

	return true;
}
