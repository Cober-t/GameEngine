#include "GameLayer.h"


namespace Cober {
	

	Game::Game() : Layer("First 2D Game!") 
	{
		m_Camera = CreateRef<GameCamera>(45.0f, 1.778f, 0.01f, 1000.0f);
	}


	void Game::OnAttach() 
	{
		RenderGlobals::Init();
		RenderGlobals::SetClearColor(10, 0, 10, 255);
		Render2D::Start();
	}


	void Game::OnDetach() 
	{
 		LOG_INFO("Detached Game Layer!");
	}


	void Game::OnUpdate(Timestep ts) 
	{
		m_Camera->OnUpdate(ts);

		Render2D::ResetStats();
		Render2D::BeginScene(m_Camera);

		// Draw anything!!
		Render2D::DrawSolidPolygon();

		Render2D::EndScene();
	}


	// void Game2D::OnEvent(SDL_Event& event) 
	// {
	// 	m_Camera->OnEvent(event);
	// }
}