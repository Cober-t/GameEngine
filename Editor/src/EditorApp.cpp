#include <Engine.h> 
#include <EntryPoint.h>

#include "EditorLayer.h"

namespace Cober {

	class EditorApp : public EngineApp {

	public:
	
		EditorApp() : EngineApp("Engine Editor")
		{
			PushLayer(new Editor());
			EngineApp::Get().SetGameState(GameState::EDITOR);
		}

		~EditorApp() {

			LOG_INFO("Editor Destructor!");
		}
	};

	EngineApp* CreateApplication()
	{
		LOG_INFO("Editor Constructor!");
		return new EditorApp();
	}
}