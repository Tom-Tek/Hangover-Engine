#pragma once

#include <QMenuBar>

#include <vector>

class EditorMenubar : public QMenuBar
{
	Q_OBJECT
public:
	explicit EditorMenubar();

private:
	QMenu* m_fileMenu;
};