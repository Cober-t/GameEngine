#include "GameLayer.h"


namespace Cober {

	Game::Game() : Layer("First 2D Game!") 
	{
		m_Camera = CreateRef<GameCamera>(45.0f, 1.778f, 0.01f, 1000.0f);
	}


	void Game::OnAttach() 
	{
		m_ActiveScene = CreateRef<Scene>();
		
		/////// Test
		// Entity 1
		Entity entity = m_ActiveScene->CreateEntity();
		entity.AddComponent<Rigidbody2D>();
		entity.GetComponent<Rigidbody2D>().type = BodyType::Dynamic;
		entity.AddComponent<BoxCollider2D>();

		// Entity 2
		Entity entity2 = m_ActiveScene->CreateEntity();
		entity2.GetComponent<TransformComponent>().position.y = -2.0f;
		entity2.AddComponent<Rigidbody2D>();
		entity2.AddComponent<BoxCollider2D>();
		entity2.AddComponent<ScriptComponent>();

		m_ActiveScene->OnSimulationStart();

		// m_ActiveScene = Scene::Load("Scene2.lua");
		// m_ActiveScene->OnSimulationStart();
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
		m_ActiveScene->OnUpdateSimulation(ts, m_Camera);
	}


	// void Game2D::OnEvent(SDL_Event& event) 
	// {
	// 	m_Camera->OnEvent(event);
	// }
}