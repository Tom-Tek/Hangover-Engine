#include "core/Application.h"

#include "editor/EditorLayer.h"
#include "editor/gui/EditorGuiLayer.h"

int main(int argc, char** argv)
{
	Application& app = Application::Singleton();
	app.PushLayer<EditorGuiLayer>(argc, argv);
	app.PushLayer<EditorLayer>(argc, argv);
	app.Run();
}