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


EngineApp* CreateApplication(AppCommandLineArgs args) 
{
	AppSpecification spec;
	spec.Name = "Game Application";
	spec.WorkingDirectory = "./";
	spec.Width = 1280;
	spec.Height = 720;
	spec.CommandLineArgs = args;

	return new GameApp(spec);
	LOG_INFO("Game Constructor!");
}