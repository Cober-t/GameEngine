#include "GameLayer.h"


namespace Cober {

	Game::Game() : Layer("First 2D Game!") 
	{
		m_Camera = CreateRef<GameCamera>(45.0f, 1.778f, 0.01f, 1000.0f);
	}


	void Game::OnAttach() 
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnSimulationStart();

		// Test
		Entity entity = m_ActiveScene->CreateEntity();
	}


	void Game::OnDetach() 
	{
		m_ActiveScene->OnSimulationStop();
		m_ActiveScene = nullptr;
		m_Camera = nullptr;
 		LOG_INFO("Detached Game Layer!");
	}


	void Game::OnUpdate(Timestep ts) 
	{
		m_ActiveScene->OnUpdateSimulation(ts, m_Camera);
	}


	// void Game2D::OnEvent(SDL_Event& event) 
	// {
	// 	m_Camera->OnEvent(event);
	// }
}