#pragma once

#include "core/Layer.h"

#include "gui/EditorTheme.h"
#include "gui/EditorWindow.h"

#include <memory>

#include <QApplication>

class EditorLayer : public Layer
{
public:
	// argc must be a reference: QApplication keeps it for its whole lifetime
	explicit EditorLayer(int& argc, char** argv);

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float deltaTime) override;

private:
	QApplication m_qapp;
	EditorTheme m_theme;
	std::unique_ptr<EditorWindow> m_window;
};
