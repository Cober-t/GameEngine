#include "GameLayer.h"


namespace Cober {

	Game::Game() : Layer("First 2D Game!") {

		// m_Camera = CreateUnique<EditorCamera>(45.0f, 1.778f, 0.01f, 1000.0f);
	}

	void Game::OnAttach() {

		// _activeScene = Scene::Load("EditorSceneTest.txt");
		// _activeScene->OnRuntimeStart(_activeScene);
	}

	void Game::OnDetach() {

		// m_ActiveScene = nullptr;
		// m_Camera = nullptr;
	}

	void Game::OnUpdate(Timestep ts) {

		bool ortho = false;
		// m_Camera->SetViewportSize(1280, 720, ortho);

		// m_Camera->OnUpdate(ts);
		// RenderGlobals::SetClearColor(10, 0, 10, 255);
		// RenderGlobals::Clear();

		// m_ActiveScene->OnUpdateRuntime(ts, _camera);

		// Render Scene (Game), when it has a cam
		// m_ActiveScene->OnUpdateRuntime(ts);
	}

	// void Game2D::OnEvent(SDL_Event& event) {

	// 	m_Camera->OnEvent(event);
	// }
}