#pragma once

#include <cstdint>
#include <format>
#include <string>

namespace
{
	uint32_t k_EditorVersion	= 101;
	uint32_t k_RuntimeVersion	= 101;

	std::string FormatVersion(const uint32_t ver)
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