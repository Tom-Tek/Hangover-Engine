#pragma once

#include <QMainWindow>
#include <QCloseEvent>

class EditorWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit EditorWindow();

private:
	void closeEvent(QCloseEvent* event) override;
	QDockWidget* AddDock(const QString& title, QWidget* content, Qt::DockWidgetArea area);
};
