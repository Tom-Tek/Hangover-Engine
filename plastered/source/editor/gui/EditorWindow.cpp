#include "EditorWindow.h"
#include "core/Globals.h"
#include "core/Application.h"

#include <string>

EditorWindow::EditorWindow()
{
	std::string version = FormatVersion(k_EditorVersion);
	std::string title = std::format("Plastered Editor - v{} / Hangover Engine", version);
	setWindowTitle(QString::fromStdString(title));
	resize(1280, 720);
}

void EditorWindow::closeEvent(QCloseEvent* event)
{
	event->accept();
	Application::Singleton().Shutdown();
}
