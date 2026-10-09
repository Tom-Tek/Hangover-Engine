#pragma once

#include <QMainWindow>
#include <QCloseEvent>

class EditorWindow : public QMainWindow
{
	Q_OBJECT
public:
	EditorWindow();

private:
	void closeEvent(QCloseEvent* event) override;
};