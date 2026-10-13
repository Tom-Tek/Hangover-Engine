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
		auto* gui = Application::Singleton().GetFirstLayer<EditorGuiLayer>();

#ifdef NDEBUG
		if (gui)
		{
			gui->ShowCrashDialog("Cannot launch without a project path.");
		}
#else
		if (gui)
			m_projectPath = gui->GetOpenFileName("Hangover Project (*.hang)", "Open Hangover Project");
#endif

		if (m_projectPath.empty())
		{
			Application::Singleton().Quit();
			return;
		}
	}
}

std::filesystem::path EditorLayer::ReadProjectPath(int argc, char** argv)
{
	if (argc > 1)
		return std::filesystem::path(argv[1]);
	return std::filesystem::path();
}