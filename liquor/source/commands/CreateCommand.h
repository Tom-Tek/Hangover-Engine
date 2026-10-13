#pragma once

#include <enact/enact.h>

#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>

// liquor create <name> <location> <template>
// Generates a new project folder at <location>/<name>, then fills in any files it is
// missing from resources/project-templates/<template>.
// {VARIABLE}s in template file names and contents are replaced, see DefineVariables
class CreateCommand : public enact::Command
{
public:
	CreateCommand();

	void Execute(std::string name, std::string location, std::string templateName);

private:
	// Variables available to templates as {NAME}
	void DefineVariables(const std::string& name);

	// Fills the freshly created project root with its folders and files
	bool PopulateProject(const std::string& name);

	// Recursively copies every file from the template that the project does not already have
	bool CopyTemplate(const std::filesystem::path& templateDir);

	// Replaces every known {VARIABLE}; unknown names and ${...} (CMake, shell) are left as they are
	std::string FormatTemplate(std::string_view text) const;

	// Paths are relative to the project root; missing parent folders are created
	bool MakeDirectory(const std::filesystem::path& path);
	bool MakeFile(const std::filesystem::path& path, std::string_view contents = {});

	std::filesystem::path m_root;
	std::unordered_map<std::string, std::string> m_variables;
};
