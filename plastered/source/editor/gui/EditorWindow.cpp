#include "EditorWindow.h"

#include "core/Application.h"
#include "core/Globals.h"

#include "editor/EditorLayer.h"

#include <QWKWidgets/widgetwindowagent.h>

#include <QTextEdit>

EditorWindow::EditorWindow()
{
	// Must come first: QWindowKit changes how Qt sizes the window
	m_windowAgent = new QWK::WidgetWindowAgent(this);
	m_windowAgent->setup(this);

	setCentralWidget(new QTextEdit(this));

	std::string version = globals::FormatVersion(globals::k_EditorVersion);
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
		m_explorerWindow = CreateGenericWindow<ExplorerWindow>(Qt::LeftDockWidgetArea);
		m_propertiesWindow = CreateGenericWindow<PropertiesWindow>(Qt::LeftDockWidgetArea);

		m_packagerWindow = new PackagerWindow();
		m_packagerWindow->hide();

		QAction* newSceneAction = m_menuBar->FileMenu->addAction("New Scene");
		connect(newSceneAction, &QAction::triggered, this, [this]()
		{
			if (auto* editorLayer = Application::Singleton().GetFirstLayer<EditorLayer>())
			{
				editorLayer->PromptCreateNewScene();
			}
		});

		QAction* packageGameAction = m_menuBar->FileMenu->addAction("Package Game");
		connect(packageGameAction, &QAction::triggered, this, [this]()
		{
			m_packagerWindow->show();
			m_packagerWindow->raise();
			m_packagerWindow->activateWindow();
		});
	}
}

void EditorWindow::closeEvent(QCloseEvent* event)
{
	event->accept();
	Application::Singleton().Quit();
}