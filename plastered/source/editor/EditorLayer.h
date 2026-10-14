#pragma once

#include "core/Layer.h"
#include "scene/Scene.h"

#include "gui/EditorTheme.h"
#include "gui/EditorWindow.h"

#include "tools/EditorApplicationFile.h"

#include <memory>
#include <filesystem>

#include <QApplication>

class EditorLayer : public Layer
{
public:
	// argc must be a reference: QApplication keeps it for its whole lifetime
	explicit EditorLayer(int& argc, char** argv);

	void OnAttach() override;
	
private:
	std::filesystem::path m_projectFile;
	EditorApplicationFile m_editorAppFile;
	std::unique_ptr<Scene> m_activeScene;

	std::filesystem::path FixProjectPath();

	// argv[1] should always be the path to the project we're opening
	std::filesystem::path ReadProjectPath(int argc, char** argv);
};
