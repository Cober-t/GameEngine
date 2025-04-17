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
		PushLayer(new Game());
		EngineApp::Get().SetGameState(EngineApp::GameState::PLAY);
	}

	~GameApp() 
	{
		LOG_INFO("Game Destructor!");
	}
};

Cober::EngineApp* Cober::CreateApplication(Cober::AppCommandLineArgs args)
{
	AppSpecification spec;
	spec.Name = "PONG";
	spec.WorkingDirectory = "C:/Users/jorge/OneDrive/Escritorio/GameEngine/Game";
	spec.Width = 640;
	spec.Height = 360;
	spec.CommandLineArgs = args;

	LOG_INFO("Game Constructor!");
	return new GameApp(spec);
}