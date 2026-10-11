#include "ExplorerWindow.h"

ExplorerWindow::ExplorerWindow()
{
	setObjectName("editor_explorerWindow");

	setWindowTitle("Explorer");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
}