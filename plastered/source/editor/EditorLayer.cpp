#include "EditorLayer.h"

#include <QDockWidget>
#include <QLabel>
#include <QMenuBar>
#include <QStatusBar>

EditorLayer::EditorLayer(int& argc, char** argv)
	: m_qapp(argc, argv)
{
	ApplyEditorTheme(m_qapp, m_theme);

	m_window = std::make_unique<EditorWindow>();
	m_window->show();
}

void EditorLayer::OnUpdate(float deltaTime)
{
	QApplication::processEvents();
	QApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);

	m_fpsAccumulator += deltaTime;
	++m_frameCount;

	if (m_fpsAccumulator >= 0.5f)
	{
		const float fps = m_frameCount / m_fpsAccumulator;
		m_window->statusBar()->showMessage(QString("%1 FPS").arg(static_cast<double>(fps), 0, 'f', 1));
		m_fpsAccumulator = 0.0f;
		m_frameCount = 0;
	}
}
