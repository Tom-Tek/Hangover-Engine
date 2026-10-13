#include "EditorGuiLayer.h"

#include "widgets/CrashDialog.h"

EditorGuiLayer::EditorGuiLayer(int& argc, char** argv) :
	m_qapp(argc, argv)
{
	QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
}

void EditorGuiLayer::OnAttach()
{
	ApplyEditorTheme(m_qapp, m_theme);

	m_editorWindow = std::make_unique<EditorWindow>();
	m_editorWindow->show();
}

void EditorGuiLayer::OnDetach()
{
	QApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
}

void EditorGuiLayer::OnUpdate(const float deltaTime)
{
	QApplication::processEvents();
	QApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
}

void EditorGuiLayer::ShowCrashDialog(const QString& message)
{
	CrashDialog dialog(message, m_editorWindow.get());
	dialog.exec();
}
