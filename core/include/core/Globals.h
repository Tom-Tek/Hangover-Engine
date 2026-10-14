#pragma once

#include <cstdint>
#include <format>
#include <string>

namespace globals
{
	inline constexpr uint16_t k_EditorVersion = 101;
	inline constexpr uint16_t k_RuntimeVersion = 101;

	inline std::string FormatVersion(const uint16_t ver)
	{
		std::string digits = std::to_string(ver);
		while (digits.length() < 3)
			digits += "0";

		return std::format(
			"{}.{}.{}",
			digits[0],
			digits[1],
			digits[2]
		);
	}

	inline constexpr uint16_t ToRawVersion(const std::string& ver)
	{
		uint16_t raw = 0;
		for (size_t i = 0; i < ver.length(); i++)
		{
			const char c = ver[i];
			if (c >= '0' && c <= '9')
				raw = static_cast<uint16_t>(raw * 10 + (c - '0'));
		}
		return raw;
	}
}