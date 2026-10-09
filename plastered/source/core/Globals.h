#pragma once

#include <iostream>
#include <string>

namespace
{
	static const unsigned int k_EditorVersion	= 10000;
	static const unsigned int k_RuntimeVersion	= 10000;

	static std::string FormatVersion(const uint32_t ver)
	{
		std::string digits = std::to_string(ver);
		return std::format(
			"{}.{}{}.{}{}",
			digits[0],
			digits[1],
			digits[2],
			digits[3],
			digits[4]
		);
	}
}