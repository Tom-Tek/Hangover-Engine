#pragma once

#include <QMainWindow>
#include <QCloseEvent>

#include "widgets/ExplorerWindow.h"
#include "widgets/PropertiesWindow.h"

class EditorWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit EditorWindow();

private:
	void closeEvent(QCloseEvent* event) override;
	QDockWidget* AddDock(const QString& title, QWidget* content, Qt::DockWidgetArea area);

	ExplorerWindow* m_explorerWindow;
	PropertiesWindow* m_propertiesWindow;
};
