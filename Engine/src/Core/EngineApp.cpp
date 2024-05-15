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

        m_GameState = GameState::PLAY;
    }


    EngineApp::~EngineApp() 
    {
		m_Window->SetEventCallback([](Event& e) {});

        Render2D::Shutdown();   // Abstract in a global Render api class in the future
        LOG_CORE_INFO("EngineApp Destructor!");
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

        if (m_GameState == GameState::EDITOR || m_GameState == GameState::RUNTIME_EDITOR) 
        {
            m_GuiLayer = new ImGuiLayer("#version 460");
            PushOverlay(m_GuiLayer);
        }
    }

    void EngineApp::Update() 
    {
        while (m_GameState == GameState::PLAY || m_GameState == GameState::EDITOR || m_GameState == GameState::RUNTIME_EDITOR)
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
        if(!m_Minimized) 
        {
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(ts);
        }

        if (m_GameState == GameState::EDITOR || m_GameState == GameState::RUNTIME_EDITOR) 
        {
            m_GuiLayer->Begin();

            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();

            m_GuiLayer->End();
        }

        m_Window->OnUpdate();
    }


    void EngineApp::Close()
    {
        m_GameState = GameState::EXIT;
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
		m_GameState = GameState::EXIT;
		return true;
	}


	bool EngineApp::OnWindowResize(WindowResizeEvent& event)
	{
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		RenderGlobals::SetViewport(event.GetWidth(), event.GetHeight());

		return false;
	}
}