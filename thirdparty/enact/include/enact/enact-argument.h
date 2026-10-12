#pragma once

#include <charconv>
#include <concepts>
#include <string>
#include <string_view>
#include <type_traits>

namespace enact
{
	// Types that can be used as command parameters
	template<typename T>
	concept ArgumentType =
		std::integral<T> ||
		std::floating_point<T> ||
		std::same_as<T, std::string> ||
		std::same_as<T, std::string_view>;

	namespace detail
	{
		template<ArgumentType T>
		constexpr std::string_view ArgumentTypeName()
		{
			if constexpr (std::same_as<T, bool>)
				return "bool";
			else if constexpr (std::same_as<T, char>)
				return "char";
			else if constexpr (std::integral<T>)
				return std::is_signed_v<T> ? "integer" : "unsigned integer";
			else if constexpr (std::floating_point<T>)
				return "number";
			else
				return "string";
		}

		inline bool EqualsIgnoreCase(std::string_view a, std::string_view b)
		{
			if (a.size() != b.size())
				return false;
			for (size_t i = 0; i < a.size(); ++i)
			{
				char lhs = a[i] >= 'A' && a[i] <= 'Z' ? a[i] - 'A' + 'a' : a[i];
				if (lhs != b[i])
					return false;
			}
			return true;
		}

		// Converts a raw command line argument into T, returns false if the text is not a valid T
		template<ArgumentType T>
		bool ParseArgument(std::string_view text, T& out)
		{
			if constexpr (std::same_as<T, bool>)
			{
				if (text == "1" || EqualsIgnoreCase(text, "true") || EqualsIgnoreCase(text, "yes"))
					out = true;
				else if (text == "0" || EqualsIgnoreCase(text, "false") || EqualsIgnoreCase(text, "no"))
					out = false;
				else
					return false;
				return true;
			}
			else if constexpr (std::same_as<T, char>)
			{
				if (text.size() != 1)
					return false;
				out = text[0];
				return true;
			}
			else if constexpr (std::integral<T> || std::floating_point<T>)
			{
				// from_chars rejects a leading '+', but it is natural to type on a command line
				if (text.size() > 1 && text[0] == '+')
					text.remove_prefix(1);

				const char* end = text.data() + text.size();
				auto [ptr, ec] = std::from_chars(text.data(), end, out);
				return ec == std::errc() && ptr == end;
			}
			else
			{
				out = T(text);
				return true;
			}
		}
	}
}
