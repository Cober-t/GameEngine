#ifndef ENGINNE_APP_H
#define ENGINNE_APP_H

#include <filesystem>
#include "Core/Project.h"
#include "Core/PathService.h"

// #include "Core.h"
#include "Core/Input.h"

#include "Core/Window.h"
#include "Core/Timestep.h"
#include "Core/LayerStack.h"
#include <Core/Profiler.h>
#include "ImGui/ImGuiLayer.h"

#include "Events/ApplicationEvents.h"
#include "Events/Event.h"
#include "Render/Render2D.h"
#include "Core/Log.h"


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
		std::string Name = "Cober";
		std::filesystem::path ProjectPath;
		std::filesystem::path ProjectRoot;
		std::filesystem::path AssetsRoot;
		std::filesystem::path StartupScene;
		uint32_t Width = 1280;
		uint32_t Height = 720;
		AppCommandLineArgs CommandLineArgs;
	};


	static std::filesystem::path ParseProjectArg(const AppCommandLineArgs& args)
	{
		for (int i = 1; i < args.Count - 1; i++)
		{
			if (std::string(args[i]) == "--project")
				return args[i + 1];
		}
		return {};
	}

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
		void OnEvent(Event& event);

		void PushLayer(Unique<Layer> layer);
		void PushOverlay(Unique<Layer> layer);

		void ProcessEvents();
		void SetMinimized(bool minimized) { m_Minimized = minimized; }
    	bool IsMinimized() const { return m_Minimized; }

		void Close();

		inline int GetFrames() { return m_TimeStep->GetFrames(); }

		Timestep& GetTimer() { return *m_TimeStep; }
		const Timestep& GetTimer() const { return *m_TimeStep; }

		ImGuiLayer* GetImGuiLayer() { return m_GuiLayer.get(); }
		const ImGuiLayer* GetImGuiLayer() const { return m_GuiLayer.get(); }

		Window& GetWindow() { return *m_Window; }
		const Window& GetWindow() const { return *m_Window; }

		void  SetDebugMode(bool debugMode)  { m_DebugMode = debugMode; }
		bool& IsDebugMode() { return m_DebugMode; }

		inline void SetGameState(GameState state) { m_GameState = state; }
		GameState GetGameState();

	private:
		// Functions
		void Run(const Timestep& ts);
		void RunEditor(const Timestep& ts);
		void RunRender(const Timestep& ts);
		
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowMinimized(WindowMinimizedEvent& e);
		bool OnWindowRestored(WindowRestoredEvent& e);

		// Properties
		AppSpecification m_Specification {};
		Profiler m_profiler {};
		GameState m_GameState = GameState::EDITOR;
		Unique<Window> m_Window {};
		Unique<ImGuiLayer> m_GuiLayer {};
		LayerStack m_LayerStack {};
		Unique<Timestep> m_TimeStep {};

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