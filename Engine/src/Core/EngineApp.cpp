#include <pch.h>
#include "Core/EngineApp.h"
#include "Events/SDLEventTranslator.h"
#include "Events/ApplicationEvents.h"

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

        m_Window = CreateUnique<Window>(
            WindowProps(m_Specification.Name, m_Specification.Width, m_Specification.Height)
        );

        RenderGlobals::Init(m_Window->GetRawWindow(), m_Window->GetContext().get()->GetDevice());
		//Render2D::Start();
    }


    EngineApp::~EngineApp() 
    {
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
        if (m_GameState == EngineApp::GameState::EDITOR ||
        m_GameState == EngineApp::GameState::RUNTIME_EDITOR)
        {
            m_GuiLayer = new ImGuiLayer();
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

    void EngineApp::OnEvent(Event& event) 
    {
        //LOG_CORE_INFO(event.GetName());
        
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(EngineApp::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(EngineApp::OnWindowResize));

        if (m_GuiLayer && !event.Handled) {
            m_GuiLayer->OnEvent(event);
        }

        if (!event.Handled)
        {
            for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
            {
                (*it)->OnEvent(event);

                if (event.Handled) {
                    break;
                }
            }
        }
    }
    
    void EngineApp::Run(Unique<Timestep>& ts)
    {
        // Process Events
        // Input::TransitionPressedKeys();
		// Input::TransitionPressedButtons();

        ProcessEvents();

        if (m_GameState == EngineApp::GameState::EXIT) {
            return;
        }

        RenderGlobals::BeginFrame();
		
        if (!m_Minimized)
        {
            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate(ts);
            }
        }

        if (m_GuiLayer &&
            (m_GameState == EngineApp::GameState::EDITOR || m_GameState == EngineApp::GameState::RUNTIME_EDITOR))
        {
            m_GuiLayer->Begin();

            for (Layer* layer : m_LayerStack) {
                layer->OnImGuiRender();
            }

            m_GuiLayer->End();
        }

        // Upload ImGui buffers before render pass begins
        RenderGlobals::ImGuiPrepareDrawData(ImGui::GetDrawData());

        // Start the render pass
        RenderGlobals::BeginMainRenderPass();

        // Scene renderer, change the code structure to rely on layers
        // ...

        // Render ImGui into the active render pass
        RenderGlobals::ImGuiRenderDrawData(ImGui::GetDrawData());

        RenderGlobals::EndFrame();

        m_Window->OnUpdate();
        
        Input::EndFrame();

        RenderGlobals::EndFrame();
    }

    void EngineApp::Close()
    {
        m_GameState = EngineApp::GameState::EXIT;
    }

    bool EngineApp::OnWindowClose(WindowCloseEvent&)
    {
        Close();
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

        if (GetGameState() != EngineApp::GameState::PLAY)
            RenderGlobals::SetViewport(event.GetWidth(), event.GetHeight());

        return false;
    }

    void EngineApp::ProcessEvents()
    {
        // In the future each layer/object could save the event on a buffer
        // and handle it one per frame on Update instead of delay all the Application
        SDL_Event rawEvent;

        while (SDL_PollEvent(&rawEvent))
        {
            if (m_GuiLayer)
                m_GuiLayer->OnSDLEvent(rawEvent);

            Input::OnEvent(rawEvent);

            auto event = TranslateSDLEvent(rawEvent);
            if (!event) {
                continue;
            }

            OnEvent(*event);
        }

        //UISystem::ProcessInputs(event);
    }

    EngineApp::GameState EngineApp::GetGameState()
    { 
        return m_GameState; 
    }
}