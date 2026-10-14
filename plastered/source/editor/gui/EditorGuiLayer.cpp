#include "EditorGuiLayer.h"

#include "widgets/CrashDialog.h"
#include "widgets/ErrorDialog.h"

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

void EditorGuiLayer::ShowErrorDialog(const QString& message, const QString& title)
{
	ErrorDialog dialog(message, title, m_editorWindow.get());
	dialog.exec();
}

std::filesystem::path EditorGuiLayer::GetOpenFileName(QString filter, QString title)
{
	const QString path = QFileDialog::getOpenFileName(
		m_editorWindow.get(),
		title,
		QString(),
		filter
	);
	return std::filesystem::path(path.toStdString());
}

std::filesystem::path EditorGuiLayer::GetOpenDirectory(QString title, QString entryDirectory)
{
	const QString path = QFileDialog::getExistingDirectory(
		m_editorWindow.get(),
		title,
		entryDirectory
	);
	return std::filesystem::path(path.toStdWString());
}