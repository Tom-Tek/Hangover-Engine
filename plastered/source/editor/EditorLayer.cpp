#include "EditorLayer.h"

EditorLayer::EditorLayer(int& argc, char** argv)
	: m_qapp(argc, argv)
{
}

void EditorLayer::OnAttach()
{
	ApplyEditorTheme(m_qapp, m_theme);

	m_window = std::make_unique<EditorWindow>();
	m_window->show();
}

void EditorLayer::OnDetach()
{
	m_window.reset();
	QApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
}

void EditorLayer::OnUpdate(float deltaTime)
{
	QApplication::processEvents();
	QApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
}
