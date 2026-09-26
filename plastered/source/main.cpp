#include "Application.h"

int main(int argc, char** argv)
{
	Application& app = Application::Singleton();
	app.Run();
}