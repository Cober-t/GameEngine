#include <Windows.h>
#include <Engine.h>
#include <EntryPoint.h>

#include "Game.h"

using namespace Cober;

class GameApp: public EngineApp
{
public:
	GameApp(const AppSpecification& specification) : EngineApp(specification) 
	{
		PushLayer(CreateUnique<Game>());
		EngineApp::SetSceneMode(EngineApp::SceneMode::PLAYING);
	}

	~GameApp() 
	{
		LOG_INFO("Game Destructor!");
	}
};


Cober::EngineApp* Cober::CreateApplication(Cober::AppCommandLineArgs args)
{
	const auto projectPath = ParseProjectArg(args);
	LOG_CORE_ASSERT(!projectPath.empty(), "Missing --project argument");

	auto project = Project::Load(projectPath);
	LOG_CORE_ASSERT(project.has_value(), "Could not load project file");

	AppSpecification spec;
	spec.Name = project->WindowTitle;
	spec.ProjectPath = project->ProjectFilePath;
	spec.ProjectRoot = project->ProjectRoot;
	spec.AssetsRoot = project->AssetsPath;
	spec.StartupScene = project->StartupScene;
	spec.Width = project->WindowWidth;
	spec.Height = project->WindowHeight;
	spec.CommandLineArgs = args;

	LOG_INFO("Game Constructor!");
	return new GameApp(spec);
}