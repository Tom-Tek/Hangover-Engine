#include "Application.h"
#include "EditorLayer.h"

int main(int argc, char** argv)
{
	Application& app = Application::Singleton();

	app.PushLayer<EditorLayer>();

	app.Run();
}