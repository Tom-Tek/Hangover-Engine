#include "EditorWindow.h"

#include "core/Application.h"
#include "core/Globals.h"

#include <QTextEdit>

EditorWindow::EditorWindow()
{
	setCentralWidget(new QTextEdit(this));

	std::string version = FormatVersion(k_EditorVersion);
	std::string title = std::format("Plastered Editor - v{}", version);
	setWindowTitle(QString::fromStdString(title));
	resize(1600, 900);

	// Widget construction
	{
		m_explorerWindow = new ExplorerWindow();
		addDockWidget(Qt::LeftDockWidgetArea, m_explorerWindow);

		m_propertiesWindow = new PropertiesWindow();
		addDockWidget(Qt::LeftDockWidgetArea, m_propertiesWindow);
	}
}

void EditorWindow::closeEvent(QCloseEvent* event)
{
	event->accept();
	Application::Singleton().Quit();
}