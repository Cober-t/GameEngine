#ifndef ENGINNE_APP_H
#define ENGINNE_APP_H

#include "Core.h"

#include "Window.h"
#include "Timestep.h"
#include "Timer.h"
#include "Layer.h"
#include "ImGui/ImGuiLayer.h"

#include "Events/ApplicationEvents.h"
#include "Events/Event.h"

#include <sol/sol.hpp>

int main(int argc, char** argv);

class ImGuiLayer;

namespace Cober {

	enum class GameState { PLAY, EDITOR, RUNTIME_EDITOR, EXIT };


	class EngineApp
	{
	public:
		EngineApp(const std::string& name = "", uint32_t width = 1280, uint32_t height = 720, bool vsync = true);
		virtual ~EngineApp();

		inline static EngineApp& Get() { return *s_Instance; }

		void Start();
		void Update();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void OnEvent(Event& event);

		void Close();

		inline void SetMinimized(bool min) { m_Minimized = min; }
		inline void SetGameState(GameState state) { m_GameState = state; }

		inline ImGuiLayer* GetImGuiLayer() { return m_GuiLayer; }
		inline Window& GetWindow() { return *m_Window; }
		inline GameState GetGameState() { return m_GameState; }

	private:
		void Run(Timestep ts);
		
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		GameState m_GameState;
		Unique<Window> m_Window;
		ImGuiLayer* m_GuiLayer;
		Layer m_LayerStack;

		sol::state lua;
		float m_LastFrameTime = 0.0f;
		bool m_Minimized = false;

	private:
		static EngineApp* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	EngineApp* CreateApplication();
}

#endif