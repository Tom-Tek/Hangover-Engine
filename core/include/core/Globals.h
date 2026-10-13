#pragma once

#include <cstdint>
#include <format>
#include <string>

namespace
{
	inline constexpr uint32_t k_EditorVersion = 101;
	inline constexpr uint32_t k_RuntimeVersion = 101;

	inline std::string FormatVersion(const uint32_t ver)
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
}