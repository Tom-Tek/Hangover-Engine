#include "core/Application.h"

#include "editor/EditorLayer.h"
#include "editor/gui/EditorGuiLayer.h"

#include "toolkit/EngineRegistration.h"

int main(int argc, char** argv)
{
	// Keeps HANGOVER_BIN pointing at the most recently launched engine
	RegisterEngine();

	Application& app = Application::Singleton();
	app.PushLayer<EditorGuiLayer>(argc, argv);
	app.PushLayer<EditorLayer>(argc, argv);
	app.Run();
}