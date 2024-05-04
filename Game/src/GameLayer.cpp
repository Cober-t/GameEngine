#include "GameLayer.h"


namespace Cober {

	Game::Game() : Layer("First 2D Game!") 
	{
		m_Camera = CreateRef<GameCamera>(45.0f, 1280, 720, 0.01f, 1000.0f);
	}


	void Game::OnAttach() 
	{
		m_Camera->SetViewportSize(1280, 720);
		m_ActiveScene = Scene::Load("Scene2.lua");
		m_ActiveScene->OnSimulationStart();
	}


	void Game::OnDetach() 
	{
		m_ActiveScene->OnSimulationStop();
		m_ActiveScene = nullptr;
		m_Camera = nullptr;
 		LOG_INFO("Detached Game Layer!");
	}


	void Game::OnUpdate(Unique<Timestep>& ts) 
	{
		RenderGlobals::SetClearColor(46, 47, 52);
		RenderGlobals::Clear();
		m_Camera->OnUpdate(ts);
		m_ActiveScene->OnUpdateSimulation(ts, m_Camera);
	}


	void Game::OnEvent(Event& event) 
	{
		m_Camera->OnEvent(event);
	}
}