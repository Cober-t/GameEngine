#include <pch.h>
#include "Core/EngineApp.h"


namespace Cober {

    EngineApp* EngineApp::s_Instance = nullptr;

    EngineApp::EngineApp(const AppSpecification& specification)
        : m_Specification(specification)
    {
        LOG_CORE_INFO("EngineApp Constructor!");

        LOG_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        // Set working directory here
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

        LOG_CORE_INFO("Current Working Path: {0}", m_Specification.WorkingDirectory);

        m_Window = CreateUnique<Window>(WindowProps(m_Specification.Name, m_Specification.Width, m_Specification.Height));
        m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });
        m_TimeStep = CreateUnique<Timestep>();

        RenderGlobals::Init();
		Render2D::Start();

        m_GameState = EngineApp::GameState::PLAY;
    }


    EngineApp::~EngineApp() 
    {
		m_Window->SetEventCallback([](Event& e) {});

        Render2D::Shutdown();   // Abstract in a global Render api class in the future
        LOG_CORE_INFO("EngineApp Destructor!");
    }


    EngineApp& EngineApp::Get() 
    {
		return *s_Instance;
    }


    void EngineApp::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }


    void EngineApp::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }


    void EngineApp::Start()
    {

        if (m_GameState == EngineApp::GameState::EDITOR || m_GameState == EngineApp::GameState::RUNTIME_EDITOR) 
        {
            m_GuiLayer = new ImGuiLayer("#version 460");
            PushOverlay(m_GuiLayer);
        }
    }

    void EngineApp::Update() 
    {
        while ( m_GameState == EngineApp::GameState::PLAY || 
                m_GameState == EngineApp::GameState::EDITOR || 
                m_GameState == EngineApp::GameState::RUNTIME_EDITOR)
        {
            m_TimeStep->Start();

            Run(m_TimeStep);

            while(m_TimeStep->GetDeltaTime() >= 1.0f)
            {
                m_TimeStep->Update();
            }

            m_TimeStep->ResetAfterOneSecond();
        }
    }

    void EngineApp::Run(Unique<Timestep>& ts)
    {
        //Process Events
        Input::TransitionPressedKeys();
		Input::TransitionPressedButtons();
        m_Window->OnUpdate();

        if(!m_Minimized) 
        {
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(ts);
        }

        if (m_GameState == EngineApp::GameState::EDITOR || m_GameState == EngineApp::GameState::RUNTIME_EDITOR) 
        {
            m_GuiLayer->Begin();

            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();

            m_GuiLayer->End();
        }
    }


    void EngineApp::Close()
    {
        m_GameState = EngineApp::GameState::EXIT;
    }


    void EngineApp::OnEvent(Event& event)
    {
        // In the future each layer/object could save the event on a buffer
        // and handle it one per frame on Update instead of delay all the Application
        EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(EngineApp::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(EngineApp::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (event.Handled) 
				break;
			(*it)->OnEvent(event);
		}
    }
  

    bool EngineApp::OnWindowClose(WindowCloseEvent& event)
	{
		m_GameState = EngineApp::GameState::EXIT;
		return true;
	}

    EngineApp::GameState EngineApp::GetGameState()
    { 
        return m_GameState; 
    }


	bool EngineApp::OnWindowResize(WindowResizeEvent& event)
	{
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;

        // In Play mode the viewport is manage by the camera
        // In the rest, the viewport is managed by the Editor Viewport
        if (EngineApp::Get().GetGameState() != EngineApp::GameState::PLAY)
        {
		    RenderGlobals::SetViewport(event.GetWidth(), event.GetHeight());
        }

		return false;
	}
}