#include <pch.h>
#include "Core/EngineApp.h"
#include "Events/SDLEventTranslator.h"
#include "Events/ApplicationEvents.h"

namespace Cober 
{
    EngineApp* EngineApp::s_Instance = nullptr;
    Unique<Window> EngineApp::s_window = nullptr;
	EngineApp::EngineState EngineApp::m_EngineState = EngineApp::EngineState::RUNNING;
    EngineApp::SceneMode   EngineApp::m_SceneMode   = EngineApp::SceneMode::PLAYING;
    
    // --------------------------------------------------------------------------------------

    EngineApp::EngineApp(const AppSpecification& specification)
        : m_Specification(specification), m_GuiLayer(nullptr)
    {
        LOG_CORE_INFO("EngineApp Constructor!");

        LOG_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        // Set working directory here
		if (!m_Specification.ProjectRoot.empty() && !m_Specification.AssetsRoot.empty()) {
			PathService::Init(m_Specification.ProjectRoot, m_Specification.AssetsRoot);
        }

        m_TimeStep = CreateUnique<Timestep>();

        s_window = CreateUnique<Window>(
            WindowProps(m_Specification.Name, m_Specification.Width, m_Specification.Height)
        );

        RenderGlobals::Init(s_window->GetRawWindow(), s_window->GetContext().get()->GetDevice());
		Render2D::Start();
    }

    // --------------------------------------------------------------------------------------

    EngineApp::~EngineApp() 
    {
        if (m_GuiLayer && EngineApp::IsEditor())
        {
            m_GuiLayer->OnDetach();
            m_GuiLayer.reset();
        }
        
        Render2D::Shutdown();   // Abstract in a global Render api class in the future
        LOG_CORE_INFO("EngineApp Destructor!");

        m_profiler.DumpProfileStatsToLog(0.02);
    }

    // --------------------------------------------------------------------------------------

    EngineApp& EngineApp::Get() 
    {
		return *s_Instance;
    }

    // --------------------------------------------------------------------------------------

    void EngineApp::PushLayer(Unique<Layer> layer)
    {
        m_LayerStack.PushLayer(std::move(layer)); // Transfer ownership
    }

    // --------------------------------------------------------------------------------------

    void EngineApp::PushOverlay(Unique<Layer> layer)
    {
        m_LayerStack.PushOverlay(std::move(layer));  // Transfer ownership
    }

    // --------------------------------------------------------------------------------------

    void EngineApp::Start()
    {
        CB_PROFILE_FUNCTION();

        if (EngineApp::IsEditor())
        {
            m_GuiLayer = CreateUnique<ImGuiLayer>();
            m_GuiLayer->OnAttach();
        }
    }

    // --------------------------------------------------------------------------------------

    void EngineApp::Update() 
    {
        while ( IsAppRunning() )
        {
            m_TimeStep->Start();
            
            if (!IsAppRunning()) {
                return;
            }
            
            Run(*m_TimeStep);

            while(m_TimeStep->GetDeltaTime() >= 1.0f)
            {
                m_TimeStep->Update();
            }
            
            m_TimeStep->ResetAfterOneSecond();
        }
    }

    // --------------------------------------------------------------------------------------

    void EngineApp::OnEvent(Event& event) 
    {
        //LOG_CORE_INFO(event.GetName());
        
        // Move to the Window class
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(EngineApp::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(EngineApp::OnWindowResize));
        dispatcher.Dispatch<WindowMinimizedEvent>(BIND_EVENT_FN(EngineApp::OnWindowMinimized));
        dispatcher.Dispatch<WindowRestoredEvent>(BIND_EVENT_FN(EngineApp::OnWindowRestored));

        if (event.Handled) {
            return;
        }

        // Editor GUI Events
        if (m_GuiLayer) {
            m_GuiLayer->OnEvent(event);
        }

        // Layers Events
        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            (*it)->OnEvent(event);
            if (event.Handled) {
                break;
            }
        }
    }
    
    // --------------------------------------------------------------------------------------

    void EngineApp::Run(const Timestep& ts)
    {
        ProcessEvents();

        RunEditor(ts);

        RunRender(ts);

        s_window->OnUpdate();
    }

    // --------------------------------------------------------------------------------------

    void EngineApp::ProcessEvents()
    {
        CB_PROFILE_FUNCTION();
        
        // In the future each layer/object could save the event on a buffer
        // and handle it one per frame on Update instead of delay all the Application
        SDL_Event rawEvent;

        while (SDL_PollEvent(&rawEvent))
        {
            if (m_GuiLayer) {
                m_GuiLayer->OnSDLEvent(rawEvent);
            }

            Input::OnEvent(rawEvent);
            //Window::OnEvent(); // TODO: Resolve Window related Events

            auto event = TranslateSDLEvent(rawEvent);
            if (!event) {
                continue;
            }

            OnEvent(*event); // EngineApp Events

            //UISystem::ProcessInputs(event);
        }

        Input::EndFrame();
    }

    // --------------------------------------------------------------------------------------

    void EngineApp::RunEditor(const Timestep& ts) 
    {
        CB_PROFILE_FUNCTION();

        if (EngineApp::IsEditor())
        {
            LOG_CORE_ASSERT(m_GuiLayer, "ImGui Layer is not created yed");
            m_GuiLayer->Begin();
            
            for (const auto& layer : m_LayerStack) {
                layer->OnImGuiRender();   
            }
            
            m_GuiLayer->End();
        }
    }

    // --------------------------------------------------------------------------------------

    void EngineApp::RunRender(const Timestep& ts) 
    {          
        CB_PROFILE_FUNCTION();
        
        RenderGlobals::BeginFrame();

        // Upload ImGui buffers before render pass begins
        if (EngineApp::IsEditor()) 
        {
            RenderGlobals::ImGuiPrepareDrawData(ImGui::GetDrawData());
        }
        

        if (!IsMinimized()) {
            for (const auto& layer : m_LayerStack) {
                layer->OnUpdate(ts);   
            }
        }
        // Start the render pass
        RenderGlobals::BeginMainRenderPass();
        
        // Render ImGui into the active render pass
        if (EngineApp::IsEditor()) 
        {
            RenderGlobals::ImGuiRenderDrawData(ImGui::GetDrawData());
        }

        RenderGlobals::EndFrame();
    }

    // --------------------------------------------------------------------------------------

    // Move to window callbaks for Application Events
    bool EngineApp::OnWindowClose(WindowCloseEvent& event)
    {
        Close();    
        return true;
    }

    // --------------------------------------------------------------------------------------

    bool EngineApp::OnWindowResize(WindowResizeEvent& event)
    {
        if (event.GetWidth() <= 0 || event.GetHeight() <= 0)
        {
            SetMinimized(true);
            return false;
        }

        SetMinimized(false);

        // if (!IsPlayMode()) {
            s_window->SetSize(event.GetWidth(), event.GetHeight());
            RenderGlobals::SetViewport(event.GetWidth(), event.GetHeight());
        // }

        return false;
    }

    // --------------------------------------------------------------------------------------

    bool EngineApp::OnWindowMinimized(WindowMinimizedEvent& e) 
    {
        SetMinimized(true);
        return true;
    }

    // --------------------------------------------------------------------------------------

    bool EngineApp::OnWindowRestored(WindowRestoredEvent& e) 
    {
        SetMinimized(false);
        return true;
    }

    // --------------------------------------------------------------------------------------
}