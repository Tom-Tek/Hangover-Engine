#include "PropertiesWindow.h"

PropertiesWindow::PropertiesWindow()
{
	setObjectName("editor_propertiesWindow");

	setWindowTitle("Properties");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
}