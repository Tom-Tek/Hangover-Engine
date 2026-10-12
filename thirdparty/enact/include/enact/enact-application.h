#pragma once

#include <concepts>
#include <memory>
#include <functional>
#include <string>
#include <unordered_map>

#include <enact/enact-command.h>

namespace enact
{
	class Application final
	{
	public:
		Application();
		Application& operator=(const Application&) = delete;

		inline static Application* Singleton = nullptr;

		int Run(int argc, char** argv);

		// Registers a class derived from Command; constructorArgs are forwarded to its constructor.
		// Argument types are deduced from the parameters of its Execute method
		template<typename T, typename... ConstructorArgs>
		void RegisterCommand(ConstructorArgs&&... constructorArgs)
		{
			static_assert(std::derived_from<T, Command>, "RegisterCommand<T> requires T to derive from enact::Command");
			constexpr bool hasExecute = requires { &T::Execute; };
			static_assert(hasExecute, "Commands must declare a single public, non-template Execute method");

			// Skipped when Execute is missing so the static_assert above is the only error reported
			if constexpr (hasExecute)
			{
				using Parameters = typename detail::FunctionTraits<decltype(&T::Execute)>::Parameters;

				std::unique_ptr<T> command = std::make_unique<T>(std::forward<ConstructorArgs>(constructorArgs)...);
				T* self = command.get();
				command->Bind([self](auto&&... values) { self->Execute(std::forward<decltype(values)>(values)...); }, Parameters{});
				InsertCommand(std::move(command));
			}
		}

		// Argument types are deduced from the callback's parameters, e.g. [](int count, std::string name) {}
		template<typename Callback>
		void BindCommand(std::string name, Callback&& callback)
		{
			InsertCommand(std::make_unique<Command>(std::move(name), std::forward<Callback>(callback)));
		}
		
		// Registers all example commands; Only call for testing if enact is properly working.
		void RegisterExampleCommands();
		

	private:
		void InsertCommand(std::unique_ptr<Command> command);
		std::string GetCommandName(int argc, char** argv);
		Command* SearchCommand(std::string commandName);

		std::unordered_map<std::string, std::unique_ptr<Command>> m_commands;
	};
}
