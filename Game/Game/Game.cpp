#include "Game.h"


Game::Game() : Layer("Game application") 
{
	m_DefaultCamera = CreateRef<GameCamera>(45.0f, EngineApp::Get().GetWindow().GetWidth(), EngineApp::Get().GetWindow().GetHeight(), 0.01f, 1000.0f);
}


void Game::OnAttach() 
{
	m_ActiveScene = Scene::Load("SceneDefault.lua");
	m_ActiveScene->OnSimulationStart();
}


void Game::OnDetach()
{
	m_ActiveScene->OnSimulationStop();
	m_ActiveScene = nullptr;
	m_DefaultCamera = nullptr;
	LOG_INFO("Detached Game application Layer!");
}


void Game::OnUpdate(Unique<Timestep>& ts) 
{
	RenderGlobals::SetClearColor(0, 0, 0);
	RenderGlobals::Clear();
	m_ActiveScene->OnUpdateSimulation(ts, m_DefaultCamera);
}


void Game::OnEvent(Event& event) 
{
	m_DefaultCamera->OnEvent(event);
}