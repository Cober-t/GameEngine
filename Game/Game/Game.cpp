#include "Game.h"


Game::Game() : Layer("Game application") 
{
	m_DefaultCamera = CreateRef<GameCamera>(45.0f, EngineApp::Get().GetWindow().GetWidth(), EngineApp::Get().GetWindow().GetHeight(), 0.01f, 1000.0f, GlobalCamera::perspective);
}


void Game::OnAttach() 
{
	m_ActiveScene = Scene::Load("SceneDefault.lua");

	// Create Framebuffer...
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
	// Resize Framebuffer...
	// Bind Framebuffer...

	RenderGlobals::SetClearColor(20, 70, 150);
	RenderGlobals::Clear();

	// Clear Framebuffer Attachments...

	m_ActiveScene->OnUpdateSimulation(ts, m_DefaultCamera);

	// Get Selected Entity...

	// Unbind Framebuffer...

	// Render Framebuffer Attachment (scene generated texture)...
}


void Game::OnEvent(Event& event) 
{
	m_ActiveScene->OnEvent(event, m_DefaultCamera);
	NativeScriptFn::OnEvent(m_ActiveScene.get(), event);
}
