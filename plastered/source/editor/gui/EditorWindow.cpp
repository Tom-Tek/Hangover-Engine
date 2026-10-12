#include "EditorWindow.h"

#include "core/Application.h"
#include "core/Globals.h"

#include <QWKWidgets/widgetwindowagent.h>

#include <QTextEdit>

EditorWindow::EditorWindow()
{
	// Must come first: QWindowKit changes how Qt sizes the window
	m_windowAgent = new QWK::WidgetWindowAgent(this);
	m_windowAgent->setup(this);

	setCentralWidget(new QTextEdit(this));

	std::string version = FormatVersion(k_EditorVersion);
	std::string title = std::format("Plastered Editor - v{}", version);
	setWindowTitle(QString::fromStdString(title));
	resize(1600, 900);

	// The menu bar lives inside the title bar, which replaces the OS one
	m_menuBar = new EditorMenubar();
	m_titleBar = new EditorTitleBar(this, m_menuBar);
	m_titleBar->Install(*m_windowAgent);
	setMenuWidget(m_titleBar);

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