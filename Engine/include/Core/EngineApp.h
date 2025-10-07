#ifndef ENGINNE_APP_H
#define ENGINNE_APP_H

#include "Core.h"
#include "Input.h"

#include "Window.h"
#include "Timestep.h"
#include "Layer.h"
#include "ImGui/ImGuiLayer.h"

#include "Events/ApplicationEvents.h"
#include "Events/Event.h"
#include "Render/Render2D.h"
#include "Log.h"


int main(int argc, char** argv);

namespace Cober {

	
	struct AppCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			LOG_CORE_ASSERT(index < Count, "There is no command line arguments to return");
			return Args[index];
		}
	};

	struct AppSpecification
	{
		std::string Name = "Cober Engine Application";
		std::string LastScene = "SceneDefault.lua";
		std::string WorkingDirectory = "./";
		uint32_t Width = 1280;
		uint32_t Height = 720;
		AppCommandLineArgs CommandLineArgs;
	};


	class CB_API EngineApp
	{
	public:
		enum class GameState { PLAY, EDITOR, RUNTIME_EDITOR, EXIT };

	public:
		EngineApp(const AppSpecification& specification);
		virtual ~EngineApp();

		static EngineApp& Get();
		const AppSpecification& GetSpecification() const { return m_Specification; }

		void Start();
		void Update();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void OnEvent(Event& event);

		void Close();

		inline int GetFrames() { return m_TimeStep->GetFrames(); }
		inline Unique<Timestep>& GetTimer() { return m_TimeStep; }
		inline void SetMinimized(bool min) { m_Minimized = min; }

		inline ImGuiLayer* GetImGuiLayer() { return m_GuiLayer; }
		inline Window& GetWindow() { return *m_Window; }
		void  SetDebugMode(bool debugMode)  { m_DebugMode = debugMode; }
		bool& IsDebugMode() { return m_DebugMode; }

		inline void SetGameState(GameState state) { m_GameState = state; }
		GameState GetGameState();

	private:
		void Run(Unique<Timestep>& ts);
		
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		AppSpecification m_Specification;
		GameState m_GameState;
		Unique<Window> m_Window;
		ImGuiLayer* m_GuiLayer;
		Layer m_LayerStack;
		Unique<Timestep> m_TimeStep;

		float m_LastFrameTime = 0.0f;
		bool m_Minimized = false;
		bool m_DebugMode = false;
		

	private:
		static EngineApp* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	EngineApp* CreateApplication(AppCommandLineArgs args);
}

#endif