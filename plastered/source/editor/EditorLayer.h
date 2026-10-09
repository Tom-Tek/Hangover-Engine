#pragma once

#include "core/Layer.h"

#include "gui/EditorTheme.h"
#include "gui/EditorWindow.h"

#include <memory>

#include <QApplication>
#include <QMainWindow>

class EditorLayer : public Layer
{
public:
	// argc must be a reference: QApplication keeps it for its whole lifetime
	EditorLayer(int& argc, char** argv);

	void OnUpdate(float deltaTime) override;

private:
	// Declared first so it is constructed before, and destroyed after, every widget
	QApplication m_qapp;
	EditorTheme m_theme;

	std::unique_ptr<EditorWindow> m_window;

	float m_fpsAccumulator = 0.0f;
	int m_frameCount = 0;
};
