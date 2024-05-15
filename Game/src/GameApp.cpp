#include <Engine.h>
#include <EntryPoint.h>

#include "GameLayer.h"


using namespace Cober;

class GameApp: public EngineApp 
{
public:

	GameApp(const AppSpecification& specification) : EngineApp(specification) 
	{
		PushLayer(new Game());
		EngineApp::Get().SetGameState(GameState::PLAY);
	}

	~GameApp() 
	{
		LOG_INFO("Game Destructor!");
	}
};


Cober::EngineApp* Cober::CreateApplication(Cober::AppCommandLineArgs args)
{
	AppSpecification spec;
	if (args.Count >= 4)
	{
		spec.Name = args[1];
		spec.WorkingDirectory = (std::string)args[2];
		spec.Width = atoi(args[3]);
		spec.Height = atoi(args[4]);
		spec.CommandLineArgs = args;
	}

	LOG_INFO("Editor Constructor!");
	return new GameApp(spec);
}