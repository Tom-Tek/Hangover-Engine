#pragma once

#include <QMainWindow>
#include <QCloseEvent>

#include "menubar/EditorMenubar.h"
#include "titlebar/EditorTitleBar.h"
#include "widgets/ExplorerWindow.h"
#include "widgets/PropertiesWindow.h"
#include "widgets/PackagerWindow.h"

namespace QWK
{
	class WidgetWindowAgent;
}

class EditorWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit EditorWindow();

private:
	template<typename T>
	requires(std::is_base_of_v<QDockWidget, T>)
	T* CreateGenericWindow(Qt::DockWidgetArea dockArea = Qt::NoDockWidgetArea)
	{
		T* window = new T();
		if (dockArea != Qt::NoDockWidgetArea)
			addDockWidget(dockArea, window);
		window->show();
		return window;
	}

	void closeEvent(QCloseEvent* event) override;
	QDockWidget* AddDock(const QString& title, QWidget* content, Qt::DockWidgetArea area);

	QWK::WidgetWindowAgent* m_windowAgent;

	EditorTitleBar* m_titleBar;
	EditorMenubar* m_menuBar;
	ExplorerWindow* m_explorerWindow;
	PropertiesWindow* m_propertiesWindow;
	PackagerWindow* m_packagerWindow;
};
