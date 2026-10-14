#include "tools/EditorApplicationFile.h"

#include <charconv>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>

namespace
{
	std::string_view Trim(std::string_view text)
	{
		const size_t first = text.find_first_not_of(" \t\r");
		if (first == std::string_view::npos)
			return {};

		const size_t last = text.find_last_not_of(" \t\r");
		return text.substr(first, last - first + 1);
	}

	// Raw text after "key =", or false if the key is not in the file
	bool FindValue(std::istream& file, std::string_view key, std::string& value)
	{
		// Every member scans from the top, so the order of keys in the file never matters
		file.clear();
		file.seekg(0);

		std::string line;
		while (std::getline(file, line))
		{
			const std::string_view text = Trim(line);

			// Blank lines, comments and [section] headers
			if (text.empty() || text[0] == '#' || text[0] == ';' || text[0] == '[')
				continue;

			const size_t equals = text.find('=');
			if (equals == std::string_view::npos || Trim(text.substr(0, equals)) != key)
				continue;

			value = Trim(text.substr(equals + 1));
			return true;
		}

		return false;
	}

	// Folders in a .hangproj file are written relative to it
	bool FormatHangPath(const std::filesystem::path& file, std::filesystem::path* path)
	{
		*path = (file.parent_path() / *path).lexically_normal();
		return true;
	}

	// Inverse of FormatHangPath; paths that can't be made relative (e.g. another drive) stay absolute
	std::string ToHangPath(const std::filesystem::path& dir, const std::filesystem::path& path)
	{
		if (path.is_relative())
			return path.generic_string();

		const std::filesystem::path relative = path.lexically_relative(dir);
		return relative.empty() ? path.generic_string() : "./" + relative.generic_string();
	}
}

template<typename TType>
bool EditorApplicationFile::ReadMember(std::istream& file, std::string_view key, TType* out)
{
	std::string value;
	if (!FindValue(file, key, value))
	{
		std::cerr << "Project file is missing '" << key << "'.\n";
		return false;
	}

	if constexpr (std::is_same_v<TType, eastl::string>)
	{
		out->assign(value.data(), value.size());
	}
	else if constexpr (std::is_same_v<TType, std::string> || std::is_same_v<TType, std::filesystem::path>)
	{
		*out = value;
	}
	else if constexpr (std::is_same_v<TType, bool>)
	{
		if (value != "true" && value != "false" && value != "1" && value != "0")
		{
			std::cerr << "Project file value '" << key << " = " << value << "' is not a boolean.\n";
			return false;
		}

		*out = value == "true" || value == "1";
	}
	else if constexpr (std::is_arithmetic_v<TType>)
	{
		// Parsed into a temporary so a malformed value never overwrites *out
		TType result{};
		const auto [end, error] = std::from_chars(value.data(), value.data() + value.size(), result);
		if (error != std::errc() || end != value.data() + value.size())
		{
			std::cerr << "Project file value '" << key << " = " << value << "' is not a number.\n";
			return false;
		}

		*out = result;
	}
	else
	{
		static_assert(!sizeof(TType), "ReadMember has no conversion for this type");
	}

	return true;
}

bool EditorApplicationFile::DeserializeFromFile(std::filesystem::path file)
{
	std::ifstream stream(file, std::ios::binary);
	if (!stream)
	{
		std::cerr << "Failed to open project file '" << file.string() << "'.\n";
		return false;
	}

	return
		ReadMember(stream, "name", &name) &&
		ReadMember(stream, "engine_version", &engineVersion) &&
		ReadMember(stream, "content_dir", &contentPath) &&
		FormatHangPath(file, &contentPath) &&
		ReadMember(stream, "source_dir", &sourcePath) &&
		FormatHangPath(file, &sourcePath);
}

bool EditorApplicationFile::SerializeToFile(std::filesystem::path dir) const
{
	const std::filesystem::path file = dir / (std::string(name.c_str()) + ".hangproj");

	std::ofstream stream(file, std::ios::binary);
	stream
		<< "[project]\n"
		<< "name = " << name.c_str() << '\n'
		<< "engine_version = " << engineVersion.c_str() << '\n'
		<< "content_dir = " << ToHangPath(dir, contentPath) << '\n'
		<< "source_dir = " << ToHangPath(dir, sourcePath) << '\n';

	if (!stream)
	{
		std::cerr << "Failed to write project file '" << file.string() << "'.\n";
		return false;
	}

	return true;
}
