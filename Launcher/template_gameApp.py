import os

GAME_CODE_HEADER_PATH = "..\\Game\\include"
GAME_CODE_SRC_PATH = "..\\Game\\src"

class GameAppTemplate():
    
    def __init__(self, name, projectPath):
        
        os.mkdir(os.path.join(projectPath, name))
        self.projectPath = projectPath
        self.projectName = name
        
        self.mainAppTemplate = self.MainAppTemplate()
        self.gameHeaderTemplate = self.GameHeaderTemplate()
        self.gameSourceTemplate = self.GameSourceTemplate()

    
    def GenerateTemplateCodes(self):
        # C:\\Users\\Jorge\\Desktop\\FirstGame\\Game\\GameApp.h
        file = open(os.path.join(self.projectPath, self.projectName, self.projectName + "App.h"), 'w')
        file.write(self.mainAppTemplate)
        file.close()
        # C:\\Users\\Jorge\\Desktop\\FirstGame\\Game\\Game.h
        file = open(os.path.join(self.projectPath, self.projectName, self.projectName + ".h"), 'w')
        file.write(self.gameHeaderTemplate)
        file.close()
        # C:\\Users\\Jorge\\Desktop\\FirstGame\\Game\\Game.cpp
        file = open(os.path.join(self.projectPath, self.projectName, self.projectName + ".cpp"), 'w')
        file.write(self.gameSourceTemplate)
        file.close()


    def MainAppTemplate(self):
        return """\
#include <Engine.h>
#include <EntryPoint.h>

#include "{1}.h"

namespace Cober {{

	class {0}: public EngineApp 
	{{
	public:

        // Change constructor in the near future to accept specification,
        // arguments send from launcher load project
		{0}() : EngineApp("{1} Application!!", 400, 200) 
		{{
			PushLayer(new {1}());
			EngineApp::Get().SetGameState(GameState::PLAY);
		}}

		~{0}() 
		{{
            LOG_INFO("{1} application Destructor!");
		}}
	}};


	EngineApp* CreateApplication() 
	{{
        LOG_INFO("{1} application Constructor!");
		return new {0}();
	}}
}}
""".format(self.projectName + "App", self.projectName)


    def GameHeaderTemplate(self):
        return """\
#ifndef {0}_H
#define {0}_H

#include <Engine.h>

namespace Cober {{

	class {0} : public Layer 
	{{
	public:
		{0}();
		virtual ~{0}() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Unique<Timestep>& ts) override;
		void OnEvent(Event& event) override;
	private:
		Ref<Scene> m_ActiveScene;
		Ref<GameCamera> m_Camera;
	}};
}}

#endif
""".format(self.projectName)


    def GameSourceTemplate(self):
        return """\
#include "{0}.h"

namespace Cober {{

	{0}::{0}() : Layer("{0} application") 
	{{
		m_Camera = CreateRef<GameCamera>(45.0f, EngineApp::Get().GetWindow().GetWidth(), EngineApp::Get().GetWindow().GetHeight(), 0.01f, 1000.0f);
	}}


	void {0}::OnAttach() 
	{{
		m_ActiveScene = Scene::Load("SceneDefault.lua");
		m_ActiveScene->OnSimulationStart();
	}}


	void {0}::OnDetach() 
	{{
		m_ActiveScene->OnSimulationStop();
		m_ActiveScene = nullptr;
		m_Camera = nullptr;
 		LOG_INFO("Detached {0} application Layer!");
	}}


	void {0}::OnUpdate(Unique<Timestep>& ts) 
	{{
		RenderGlobals::SetClearColor(46, 47, 52);
		RenderGlobals::Clear();
		m_Camera->OnUpdate(ts);
		m_ActiveScene->OnUpdateSimulation(ts, m_Camera);
	}}


	void {0}::OnEvent(Event& event) 
	{{
		m_Camera->OnEvent(event);
	}}
}}
""".format(self.projectName)