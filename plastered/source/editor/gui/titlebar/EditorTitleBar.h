#pragma once

#include <QWidget>

class QLabel;
class QMenuBar;
class TitleBarButton;

namespace QWK
{
	class WidgetWindowAgent;
}

// Replaces the OS title bar: menu bar, window title and minimize/maximize/close in one strip.
// On macOS the native traffic-light buttons are kept and ours are not created.
class EditorTitleBar : public QWidget
{
	Q_OBJECT
public:
	EditorTitleBar(QWidget* window, QMenuBar* menuBar);

	// Tells QWindowKit which parts are buttons and which are the draggable area
	void Install(QWK::WidgetWindowAgent& agent);

protected:
	// Keeps the title and the maximize glyph in sync with the window
	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	QWidget* m_window;
	QMenuBar* m_menuBar;
	QLabel* m_titleLabel;

	TitleBarButton* m_minimizeButton = nullptr;
	TitleBarButton* m_maximizeButton = nullptr;
	TitleBarButton* m_closeButton = nullptr;
};
