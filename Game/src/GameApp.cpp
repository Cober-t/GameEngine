#include <Engine.h>
#include <EntryPoint.h>

#include "GameLayer.h"


namespace Cober {


	class GameApp: public EngineApp 
	{
	public:

		GameApp() : EngineApp("Game Application!!") 
		{
			PushLayer(new Game());
			EngineApp::Get().SetGameState(GameState::PLAY);
		}

		~GameApp() 
		{
            LOG_INFO("Game Destructor!");
		}
	};


	EngineApp* CreateApplication() 
	{
        LOG_INFO("Game Constructor!");
		return new GameApp();
	}
}