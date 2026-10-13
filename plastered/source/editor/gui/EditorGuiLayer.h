#pragma once

#include "core/Layer.h"

#include "EditorTheme.h"
#include "EditorWindow.h"

#include <memory>

#include <QString>
#include <QApplication>
#include <QFileDialog>

class EditorGuiLayer : public Layer
{
public:
	EditorGuiLayer(int& argc, char** argv);

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(const float deltaTime) override;

	// Blocks until the user closes the dialog
	void ShowCrashDialog(const QString& message);
	
	std::filesystem::path GetOpenFileName(QString extension, QString title = "Open File");

private:
	QApplication m_qapp;
	EditorTheme m_theme;
	std::unique_ptr<EditorWindow> m_editorWindow = nullptr;
};