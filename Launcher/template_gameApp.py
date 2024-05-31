import os

GAME_CODE_HEADER_PATH = "..\\Game\\include"
GAME_CODE_SRC_PATH = "..\\Game\\src"

class GameAppTemplate():
    
    def __init__(self, name, projectPath, screenWidth, screenHeight):
        os.mkdir(os.path.normpath(os.path.join(projectPath, name)))
        self.projectPath = projectPath
        self.projectName = name
        self.screenWidth = screenWidth
        self.screenHeight = screenHeight
        
        self.mainAppTemplate = self.MainAppTemplate()
        self.gameHeaderTemplate = self.GameHeaderTemplate()
        self.gameSourceTemplate = self.GameSourceTemplate()

    
    def GenerateTemplateCodes(self):
        # C:\\Users\\Jorge\\Desktop\\FirstGame\\Game\\GameApp.h
        file = open(os.path.join(self.projectPath, self.projectName, self.projectName + "App.cpp"), 'w')
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
        return f"""\
#include <Engine.h>
#include <EntryPoint.h>

#include "{self.projectName}.h"

using namespace Cober;


class {self.projectName}App: public EngineApp
{{
public:

	{self.projectName}App(const AppSpecification& specification) : EngineApp(specification) 
	{{
		PushLayer(new {self.projectName}());
		EngineApp::Get().SetGameState(EngineApp::GameState::PLAY);
	}}

	~{self.projectName}App() 
	{{
		LOG_INFO("{self.projectName} Destructor!");
	}}
}};

Cober::EngineApp* Cober::CreateApplication(Cober::AppCommandLineArgs args)
{{
	AppSpecification spec;
	spec.Name = "{self.projectName}";
	spec.WorkingDirectory = "{self.projectPath}";
	spec.Width = {self.screenWidth};
	spec.Height = {self.screenHeight};
	spec.CommandLineArgs = args;

	LOG_INFO("{self.projectName} Constructor!");
	return new {self.projectName}App(spec);
}}"""


    def GameHeaderTemplate(self):
        return f"""\
#ifndef {self.projectName}_H
#define {self.projectName}_H

#include <Engine.h>

using namespace Cober;


class {self.projectName} : public Layer 
{{
public:
	{self.projectName}();
	virtual ~{self.projectName}() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Unique<Timestep>& ts) override;
	void OnEvent(Event& event) override;
private:
	Ref<Scene> m_ActiveScene;
	Ref<Camera> m_Camera;
}};

#endif
"""


    def GameSourceTemplate(self):
        return f"""\
#include "{self.projectName}.h"


{self.projectName}::{self.projectName}() : Layer("{self.projectName} application") 
{{
	m_Camera = CreateRef<GameCamera>(45.0f, EngineApp::Get().GetWindow().GetWidth(), EngineApp::Get().GetWindow().GetHeight(), 0.01f, 1000.0f);
}}


void {self.projectName}::OnAttach() 
{{
	m_ActiveScene = Scene::Load("SceneDefault.lua");
	m_ActiveScene->OnSimulationStart();
}}


void {self.projectName}::OnDetach() 
{{
	m_ActiveScene->OnSimulationStop();
	m_ActiveScene = nullptr;
	m_Camera = nullptr;
	LOG_INFO("Detached {self.projectName} application Layer!");
}}


void {self.projectName}::OnUpdate(Unique<Timestep>& ts) 
{{
	RenderGlobals::SetClearColor(46, 47, 52);
	RenderGlobals::Clear();
	m_Camera->OnUpdate(ts);
	m_ActiveScene->OnUpdateSimulation(ts, m_Camera);
}}


void {self.projectName}::OnEvent(Event& event) 
{{
	m_Camera->OnEvent(event);
}}
"""