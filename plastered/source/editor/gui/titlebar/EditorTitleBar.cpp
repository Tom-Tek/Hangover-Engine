#include "EditorTitleBar.h"

#include "TitleBarButton.h"

#include <QWKWidgets/widgetwindowagent.h>

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>

namespace
{
	constexpr int k_TitleBarHeight = 30;
	constexpr int k_ButtonWidth = 46;

	// Room left for the native close/minimize/zoom buttons on macOS
	[[maybe_unused]] constexpr int k_MacSystemButtonsWidth = 75;
}

EditorTitleBar::EditorTitleBar(QWidget* window, QMenuBar* menuBar)
	: m_window(window)
	, m_menuBar(menuBar)
{
	setObjectName("editor_titleBar");
	setAttribute(Qt::WA_StyledBackground); // lets the stylesheet paint our background
	setFixedHeight(k_TitleBarHeight);

	auto* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

#ifdef Q_OS_MACOS
	layout->addSpacing(k_MacSystemButtonsWidth);
#endif

	layout->addWidget(m_menuBar, 0, Qt::AlignVCenter);

	m_titleLabel = new QLabel(m_window->windowTitle());
	m_titleLabel->setObjectName("editor_titleBarTitle");
	m_titleLabel->setAlignment(Qt::AlignCenter);
	layout->addWidget(m_titleLabel, 1);

#ifndef Q_OS_MACOS
	m_minimizeButton = new TitleBarButton(TitleBarButton::Kind::Minimize);
	m_maximizeButton = new TitleBarButton(TitleBarButton::Kind::Maximize);
	m_closeButton = new TitleBarButton(TitleBarButton::Kind::Close);

	for (TitleBarButton* button : { m_minimizeButton, m_maximizeButton, m_closeButton })
	{
		button->setFixedSize(k_ButtonWidth, k_TitleBarHeight);
		layout->addWidget(button);
	}

	connect(m_minimizeButton, &QPushButton::clicked, m_window, &QWidget::showMinimized);
	connect(m_maximizeButton, &QPushButton::clicked, this, [this]
	{
		if (m_window->isMaximized())
			m_window->showNormal();
		else
			m_window->showMaximized();
	});
	connect(m_closeButton, &QPushButton::clicked, m_window, &QWidget::close);
#endif

	m_window->installEventFilter(this);
}

void EditorTitleBar::Install(QWK::WidgetWindowAgent& agent)
{
	agent.setTitleBar(this);

	// Everything else in the bar is a drag area, so the menu must opt back in to receive clicks
	agent.setHitTestVisible(m_menuBar, true);

#ifdef Q_OS_MACOS
	agent.setSystemButtonAreaCallback([](const QSize& size)
	{
		return QRect(0, 0, k_MacSystemButtonsWidth, size.height());
	});
#else
	// Lets Windows 11 show snap layouts when hovering the maximize button
	agent.setSystemButton(QWK::WindowAgentBase::Minimize, m_minimizeButton);
	agent.setSystemButton(QWK::WindowAgentBase::Maximize, m_maximizeButton);
	agent.setSystemButton(QWK::WindowAgentBase::Close, m_closeButton);
#endif
}

bool EditorTitleBar::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == m_window)
	{
		if (event->type() == QEvent::WindowTitleChange)
			m_titleLabel->setText(m_window->windowTitle());
		else if (event->type() == QEvent::WindowStateChange && m_maximizeButton)
			m_maximizeButton->SetMaximized(m_window->isMaximized());
	}

	return QWidget::eventFilter(watched, event);
}
