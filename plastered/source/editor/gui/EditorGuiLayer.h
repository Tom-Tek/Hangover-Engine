#pragma once

#include "core/Layer.h"

#include "EditorTheme.h"
#include "EditorWindow.h"

#include <memory>
#include <QApplication>

class EditorGuiLayer : public Layer
{
public:
	EditorGuiLayer(int& argc, char** argv);

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(const float deltaTime) override;

	// Blocks until the user closes the dialog
	void ShowCrashDialog(const QString& message);

private:
	QApplication m_qapp;
	EditorTheme m_theme;
	std::unique_ptr<EditorWindow> m_editorWindow = nullptr;
};