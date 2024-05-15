#include <Engine.h> 
#include <EntryPoint.h>

#include "EditorLayer.h"

namespace Cober {

	class EditorApp : public EngineApp {

	public:
	
		EditorApp(const AppSpecification& specification) : EngineApp(specification)
		{
			EngineApp::Get().SetGameState(GameState::EDITOR);
			PushLayer(new Editor());
		}

		~EditorApp() {

			LOG_INFO("Editor Destructor!");
		}
	};

	EngineApp* CreateApplication(AppCommandLineArgs args)
	{
		AppSpecification spec;
		spec.Name = "EDITOR";
		spec.WorkingDirectory = "./";
		spec.Width = 1280;
		spec.Height = 720;
		spec.CommandLineArgs = args;

		LOG_INFO("Editor Constructor!");
		return new EditorApp(spec);
	}
}
