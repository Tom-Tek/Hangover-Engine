#include "core/Globals.h"

#include "commands/CreateCommand.h"

#include "toolkit/EngineRegistration.h"

#include <enact/enact.h>

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
	// Keeps HANGOVER_BIN pointing at the most recently launched engine
	RegisterEngine();

	enact::Application app;
	app.BindCommand("version", []()
	{
		std::cout << "Liquor runtime v" << FormatVersion(k_RuntimeVersion) << '\n';
	});

	app.RegisterCommand<CreateCommand>();

	return app.Run(argc, argv);
}
