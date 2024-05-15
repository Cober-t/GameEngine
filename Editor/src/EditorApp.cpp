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
		if (args.Count >= 4)
		{
			spec.Name = args[1];
			spec.WorkingDirectory = (std::string)args[2];
			spec.Width = atoi(args[3]);
			spec.Height = atoi(args[4]);
			spec.CommandLineArgs = args;
		}

		LOG_INFO("Editor Constructor!");
		return new EditorApp(spec);
	}
}