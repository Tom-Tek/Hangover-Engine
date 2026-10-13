#pragma once

#include <QMenuBar>

#include <vector>
#include <iostream>

class EditorMenubar : public QMenuBar
{
	Q_OBJECT
public:
	explicit EditorMenubar();

	QMenu* FileMenu;
};