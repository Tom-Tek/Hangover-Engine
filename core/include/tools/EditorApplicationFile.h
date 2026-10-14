#pragma once

#include "core/Core.h"

#include <filesystem>
#include <iosfwd>
#include <string_view>
#include <EASTL/string.h>

class CORE_API EditorApplicationFile
{
public:
	EditorApplicationFile() = default;

	eastl::string name;
	eastl::string engineVersion;
	std::filesystem::path contentPath;
	std::filesystem::path sourcePath;

	// False if the file could not be opened or any member was missing or malformed
	bool DeserializeFromFile(std::filesystem::path file);
	// Writes <dir>/<name>.hangproj, folders relative to dir. False if the file could not be written
	bool SerializeToFile(std::filesystem::path dir) const;

private:
	// Finds "key = value" in the .hangproj file and writes it to *out as TType.
	// Returns false (leaving *out untouched) if the key is missing or the value doesn't convert.
	template<typename TType>
	bool ReadMember(std::istream& file, std::string_view key, TType* out);
};