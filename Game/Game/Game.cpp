#include "Game.h"


Game::Game() : Layer("Game application") 
{
	m_MousePosition = glm::vec2(0.0f);
	float screenWidth = EngineApp::Get().GetWindow().GetWidth();
	float screenHeight = EngineApp::Get().GetWindow().GetHeight();
	m_DefaultCamera = CreateRef<GameCamera>(45.0f, screenWidth, screenHeight, 0.01f, 1000.0f, GlobalCamera::perspective);

	// Create Framebuffer...
	m_Fbo = Framebuffer::Create(m_DefaultCamera->GetSettings().width, m_DefaultCamera->GetSettings().height);
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
	// Bind Framebuffer...
	m_Fbo->Bind();

	RenderGlobals::SetClearColor(20, 70, 150);
	RenderGlobals::Clear();

	// Clear Framebuffer Attachments...
	m_Fbo->ClearAttachment(1, -1);

	m_ActiveScene->OnUpdateSimulation(ts, m_DefaultCamera);

	// Get Selected Entity using inverted coordinates...
	int pixelData = m_Fbo->ReadPixel(1, m_MousePosition.x, EngineApp::Get().GetWindow().GetHeight() - m_MousePosition.y);
	LOG_WARNING("{0} {1} {2}", pixelData, m_MousePosition.x, m_MousePosition.y);

	// Unbind Framebuffer...
	m_Fbo->Unbind();

	// Render Framebuffer Attachment (scene generated texture)...
	Cober::Render2D::DrawFramebuffer(m_Fbo);
}


void Game::OnEvent(Event& event) 
{
	if (event.GetEventType() == EventType::MouseMoved)
	{
		m_MousePosition.x = static_cast<MouseMovedEvent&>(event).GetX();
		m_MousePosition.y = static_cast<MouseMovedEvent&>(event).GetY();
	}

	if (event.GetEventType() == EventType::WindowResize)
	{
		// Resize Framebuffer...
		float screenWidth = static_cast<WindowResizeEvent&>(event).GetWidth();
		float screenHeight = static_cast<WindowResizeEvent&>(event).GetHeight();
		m_Fbo->Resize(screenWidth, screenHeight);
	}

	m_ActiveScene->OnEvent(event, m_DefaultCamera);
	NativeScriptFn::OnEvent(m_ActiveScene.get(), event);
}
