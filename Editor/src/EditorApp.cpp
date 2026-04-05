#include <Engine.h> 
#include <EntryPoint.h>

#include "EditorLayer.h"

namespace Cober {

	class EditorApp : public EngineApp 
	{
	public:
		EditorApp(const AppSpecification& specification) : EngineApp(specification)
		{
			EngineApp::SetSceneMode(EngineApp::SceneMode::EDITOR);

			PushLayer(CreateUnique<Editor>());
		}

		~EditorApp() 
		{
			LOG_INFO("Editor Destructor!");
			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
			_CrtDumpMemoryLeaks();
		}
	};

	EngineApp* CreateApplication(AppCommandLineArgs args)
	{
		
		AppSpecification spec;

		const auto projectPath = ParseProjectArg(args);
		if (!projectPath.empty())
		{
			auto project = Project::Load(projectPath);
			LOG_CORE_ASSERT(project.has_value(), "Could not load project file");

			spec.Name = project->WindowTitle;
			spec.ProjectPath = project->ProjectFilePath;
			spec.ProjectRoot = project->ProjectRoot;
			spec.AssetsRoot = project->AssetsPath;
			spec.StartupScene = project->StartupScene;
			spec.Width = project->WindowWidth;
			spec.Height = project->WindowHeight;
			spec.CommandLineArgs = args;
		}

		LOG_INFO("Editor Constructor!");
		return new EditorApp(spec);
	}
}