#include "Application.h"
#include "editor/EditorLayer.h"

int main(int argc, char** argv)
{
	Application& app = Application::Singleton();

	app.PushLayer<EditorLayer>(argc, argv);

	app.Run();
}