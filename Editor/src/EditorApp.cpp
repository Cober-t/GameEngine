#include <Engine.h> 
#include <EntryPoint.h>

#include "EditorLayer.h"

namespace Cober {

	class Editor : public EngineApp {
	public:
		Editor() : EngineApp("Engine Editor")
		{
			PushLayer(new EditorLayer());
			EngineApp::Get().SetGameState(GameState::EDITOR);
		}

		~Editor() {
			//LOG_INFO("Editor Destructor Called!");
		}
	};

	EngineApp* CreateApplication()
	{
		return new Editor();
	}
}