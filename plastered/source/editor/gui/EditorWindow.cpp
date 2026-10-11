#include "EditorWindow.h"

#include "core/Application.h"
#include "core/Globals.h"

EditorWindow::EditorWindow()
{
	std::string version = FormatVersion(k_EditorVersion);
	std::string title = std::format("Plastered Editor - v{}", version);
	setWindowTitle(QString::fromStdString(title));
	resize(1600, 900);
}

void EditorWindow::closeEvent(QCloseEvent* event)
{
	event->accept();
	Application::Singleton().Quit();
}