#include <pch.h>
#include "Core/EngineApp.h"

namespace Cober {

    EngineApp* EngineApp::s_Instance = nullptr;

    EngineApp::EngineApp(const AppSpecification& specification)
        : m_Specification(specification), m_GameState(GameState::EDITOR), m_GuiLayer(nullptr)
    {
        LOG_CORE_INFO("EngineApp Constructor!");

        LOG_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        // Set working directory here
		if (!m_Specification.ProjectRoot.empty() && !m_Specification.AssetsRoot.empty()) {
			PathService::Init(m_Specification.ProjectRoot, m_Specification.AssetsRoot);
        }

        m_TimeStep = CreateUnique<Timestep>();

        /// TODO: CREATE WINDOW WITH SDL3
        m_Window = CreateUnique<Window>(
            WindowProps(m_Specification.Name, m_Specification.Width, m_Specification.Height)
        );

        /// TODO: Process Events from SDL  (EventHandler)// EventHandler::Get()->ProcessEvents(event);
        // m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

        /// TODO: RENDER PIPELINE WITH SDL3
        RenderGlobals::Init();
        // TEST
        RenderGlobals::SetClearColor({0.8f, 0.3f, 0.1f, 1.0f});
		//Render2D::Start();
    }


    EngineApp::~EngineApp() 
    {
		// m_Window->SetEventCallback([](Event& e) {});

        // Render2D::Shutdown();   // Abstract in a global Render api class in the future
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
        // Input::TransitionPressedKeys();
		// Input::TransitionPressedButtons();

        ProcessEvents();

        if (m_GameState == EngineApp::GameState::EXIT)
            return;

        if (!m_Minimized)
        {
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(ts);
        }

        if (m_GuiLayer &&
            (m_GameState == EngineApp::GameState::EDITOR || m_GameState == EngineApp::GameState::RUNTIME_EDITOR))
        {
            m_GuiLayer->Begin();

            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();

            m_GuiLayer->End();
        }

        m_Window->OnUpdate();
        
        Input::EndFrame();

        // if (m_GameState == EngineApp::GameState::EDITOR || m_GameState == EngineApp::GameState::RUNTIME_EDITOR) 
        // {
        //     m_GuiLayer->Begin();

        //     for (Layer* layer : m_LayerStack)
        //         layer->OnImGuiRender();

        //     m_GuiLayer->End();
        // }
    }


    void EngineApp::Close()
    {
        m_GameState = EngineApp::GameState::EXIT;
    }


    void EngineApp::ProcessEvents()
    {
        // In the future each layer/object could save the event on a buffer
        // and handle it one per frame on Update instead of delay all the Application
        // EventDispatcher dispatcher(event);
		// dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(EngineApp::OnWindowClose));
        // dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(EngineApp::OnWindowResize));

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            Input::OnEvent(event);

            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                {
                    Close();
                    break;
                }

                case SDL_EVENT_WINDOW_RESIZED:
                {
                    const int width = event.window.data1;
                    const int height = event.window.data2;

                    if (width == 0 || height == 0)
                    {
                        m_Minimized = true;
                    }
                    else
                    {
                        m_Minimized = false;
                        RenderGlobals::SetViewport(width, height);
                    }
                    break;
                }

                default:
                    break;
            }
            //LOG_CORE_INFO(event.key.raw);
        }

        //UISystem::ProcessInputs(event);

        /// TODO: Process Events from SDL  (EventHandler)
        // EventHandler::Get()->ProcessEvents(event);

        // for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        // {
        //     if (event.Handled) 
        //         break;
        //     (*it)->OnEvent(event);
        // }
    }
  

    bool EngineApp::OnWindowClose(WindowCloseEvent& event)
	{
		// m_GameState = EngineApp::GameState::EXIT;
		return true;
	}

    EngineApp::GameState EngineApp::GetGameState()
    { 
        return m_GameState; 
    }


	bool EngineApp::OnWindowResize(WindowResizeEvent& event)
	{
		// if (event.GetWidth() == 0 || event.GetHeight() == 0)
		// {
		// 	m_Minimized = true;
		// 	return false;
		// }

		// m_Minimized = false;

        // // In Play mode the viewport is manage by the camera
        // // In the rest, the viewport is managed by the Editor Viewport
        // if (EngineApp::Get().GetGameState() != EngineApp::GameState::PLAY)
        // {
		//     RenderGlobals::SetViewport(event.GetWidth(), event.GetHeight());
        // }

		return false;
	}
}