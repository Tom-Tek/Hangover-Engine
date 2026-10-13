#pragma once

#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <enact/enact-argument.h>

namespace enact
{
	namespace detail
	{
		template<typename... Ts>
		struct TypeList {};

		// Deduces the parameter list of a lambda, functor or function pointer
		template<typename T>
		struct FunctionTraits : FunctionTraits<decltype(&T::operator())> {};

		template<typename R, typename... Args>
		struct FunctionTraits<R(Args...)> { using Parameters = TypeList<std::decay_t<Args>...>; };

		template<typename R, typename... Args>
		struct FunctionTraits<R(*)(Args...)> : FunctionTraits<R(Args...)> {};

		template<typename R, typename... Args>
		struct FunctionTraits<R(*)(Args...) noexcept> : FunctionTraits<R(Args...)> {};

		template<typename C, typename R, typename... Args>
		struct FunctionTraits<R(C::*)(Args...)> : FunctionTraits<R(Args...)> {};

		template<typename C, typename R, typename... Args>
		struct FunctionTraits<R(C::*)(Args...) const> : FunctionTraits<R(Args...)> {};

		template<typename C, typename R, typename... Args>
		struct FunctionTraits<R(C::*)(Args...) noexcept> : FunctionTraits<R(Args...)> {};

		template<typename C, typename R, typename... Args>
		struct FunctionTraits<R(C::*)(Args...) const noexcept> : FunctionTraits<R(Args...)> {};

		template<typename T>
		bool ParseInto(std::string_view text, T& out, size_t index, std::optional<std::string>& error)
		{
			if (ParseArgument(text, out))
				return true;
			error = "Argument " + std::to_string(index + 1) + " ('" + std::string(text) + "') is not a valid " + std::string(ArgumentTypeName<T>()) + ".";
			return false;
		}
	}

	// Base class for commands. Derive from it, pass the command name to the constructor and declare
	// a public Execute method; its parameters define the command's arguments, e.g. void Execute(int a, int b)
	class Command
	{
	public:
		// Parses the raw arguments and runs the callback, returns an error message on failure
		using Invoker = std::function<std::optional<std::string>(std::span<const std::string_view>)>;

		explicit Command(std::string name)
			: m_name(std::move(name))
		{}

		template<typename Callback>
		Command(std::string name, Callback callback)
			: m_name(std::move(name))
		{
			Bind(std::move(callback), typename detail::FunctionTraits<std::decay_t<Callback>>::Parameters{});
		}

		virtual ~Command() = default;
		Command(const Command&) = delete;
		Command& operator=(const Command&) = delete;

		std::string GetUsage() const;
		const std::string& GetName() const { return m_name; }

	private:
		friend class Application;

		std::optional<std::string> Invoke(std::span<const std::string_view> args);

		template<typename Callback, typename... Args>
		void Bind(Callback callback, detail::TypeList<Args...>)
		{
			static_assert((ArgumentType<Args> && ...),
				"Command parameters must be bool, char, an integer type, a floating point type, std::string or std::string_view");

			m_parameterTypes = { detail::ArgumentTypeName<Args>()... };
			m_invoker = [callback = std::move(callback)](std::span<const std::string_view> args) mutable -> std::optional<std::string>
			{
				if (args.size() != sizeof...(Args))
					return "Expected " + std::to_string(sizeof...(Args)) + " argument(s) but got " + std::to_string(args.size()) + ".";

				return [&]<size_t... I>(std::index_sequence<I...>) -> std::optional<std::string>
				{
					std::tuple<Args...> values;
					std::optional<std::string> error;
					if (!(detail::ParseInto(args[I], std::get<I>(values), I, error) && ...))
						return error;

					std::invoke(callback, std::move(std::get<I>(values))...);
					return std::nullopt;
				}(std::index_sequence_for<Args...>{});
			};
		}

		std::string m_name;
		std::vector<std::string_view> m_parameterTypes;
		Invoker m_invoker;
	};
}
