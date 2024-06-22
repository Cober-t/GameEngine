#include "Game.h"


Game::Game() : Layer("Game application") 
{
	m_DefaultCamera = CreateRef<GameCamera>(45.0f, EngineApp::Get().GetWindow().GetWidth(), EngineApp::Get().GetWindow().GetHeight(), 0.01f, 1000.0f, GlobalCamera::perspective);
}


void Game::OnAttach() 
{
	m_ActiveScene = Scene::Load("SceneDefault.lua");
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
	RenderGlobals::SetClearColor(20.0f, 70.0f, 150.0f);
	RenderGlobals::Clear();
	m_ActiveScene->OnUpdateSimulation(ts, m_DefaultCamera);
}


void Game::OnEvent(Event& event) 
{
	m_ActiveScene->OnEvent(event, m_DefaultCamera);
	NativeScriptFn::OnEvent(m_ActiveScene.get(), event);
}
