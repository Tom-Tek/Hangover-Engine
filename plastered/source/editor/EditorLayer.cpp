#include "EditorLayer.h"

#include "core/Application.h"
#include "core/Globals.h"
#include "tools/Paths.h"

#include "gui/EditorGuiLayer.h"

#include <iostream>

EditorLayer::EditorLayer(int& argc, char** argv) : 
	m_projectFile(ReadProjectPath(argc, argv))
{
}

void EditorLayer::OnAttach()
{
	// Ensure we have a valid project path; on debug mode ask the user to find the .hangproj project
	if (m_projectFile.empty())
		m_projectFile = FixProjectPath();

	if (!m_editorAppFile.DeserializeFromFile(m_projectFile))
	{
		if (auto* gui = Application::Singleton().GetFirstLayer<EditorGuiLayer>())
			gui->ShowCrashDialog("Critical; failed to deserialized .hangproj project file.\n  File may be corrupted.");
		Application::Singleton().Quit();
	}
	
	if (globals::ToRawVersion(m_editorAppFile.engineVersion.c_str()) != globals::k_EditorVersion)
	{
		if (auto* gui = Application::Singleton().GetFirstLayer<EditorGuiLayer>())
			gui->ShowCrashDialog("Version mismatch; version merging is currently not supported in hangover.");
		Application::Singleton().Quit();
	}


}

void EditorLayer::PromptCreateNewScene()
{
	if (auto* gui = Application::Singleton().GetFirstLayer<EditorGuiLayer>())
	{
		std::filesystem::path path = gui->GetOpenDirectory("Save New Scene", QString(m_editorAppFile.contentPath.c_str()));

		if (path.empty())
			return; // It's annoying to throw an error message when just cancelling; silently return instead

		if (!paths::IsInsideDirectory(path, m_editorAppFile.contentPath))
			gui->ShowErrorDialog("Scenes must be saved inside the project's content folder.");
	}
}

std::filesystem::path EditorLayer::FixProjectPath()
{
	std::filesystem::path path = std::filesystem::path();

	auto* gui = Application::Singleton().GetFirstLayer<EditorGuiLayer>();

#ifdef NDEBUG
	if (gui)
		gui->ShowCrashDialog("Cannot launch without a project path.");
#else
	if (gui)
		path = gui->GetOpenFileName("Hangover Project (*.hangproj)", "Open Hangover Project");
#endif

	if (path.empty())
		Application::Singleton().Quit();
	return path;
}

std::filesystem::path EditorLayer::ReadProjectPath(int argc, char** argv)
{
	if (argc > 1)
		return std::filesystem::path(argv[1]);
	return std::filesystem::path();
}