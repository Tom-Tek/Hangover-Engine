#include "EditorLayer.h"

#include "core/Application.h"
#include "gui/EditorGuiLayer.h"

EditorLayer::EditorLayer(int& argc, char** argv) : 
	m_projectPath(ReadProjectPath(argc, argv))
{
}

void EditorLayer::OnAttach()
{
	// Ensure we have a valid project path; on debug mode ask the user to find the .hang project
	if (m_projectPath.empty())
	{
#ifndef NDEBUG
		
#else
		if (auto* gui = Application::Singleton().GetFirstLayer<EditorGuiLayer>())
		{
			gui->ShowCrashDialog("Cannot launch with project path.");
			Application::Singleton().Quit();
		}
		return;
#endif
	}
}

std::filesystem::path EditorLayer::ReadProjectPath(int argc, char** argv)
{
	if (argc > 1)
		return std::filesystem::path(argv[1]);
	return std::filesystem::path();
}