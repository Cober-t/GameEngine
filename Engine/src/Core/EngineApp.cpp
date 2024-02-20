#include "Core/EngineApp.h"
#include "Core/Log.h"

namespace Cober {

    EngineApp* EngineApp::s_Instance = nullptr;
    
    EngineApp::EngineApp(const std::string& name, uint32_t width, uint32_t height, bool vsync)
    {
        LOG_CORE_INFO("EngineApp Constructor!");
        DEBUG = false;
        GAME_2D = false;
        PHYSICS_2D = false;

        LOG_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
        
		// Renderer::Init();
		// ScriptEngine::Init();

        m_Window = CreateUnique<Window>(WindowProps(name, width, height, vsync));
        // m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        // m_AssetManager = CreateUnique<AssetManager>();

        m_GameState = GameState::PLAY;
    }

    EngineApp::~EngineApp() {

        // ScriptEngine::Shutdown();
		// Renderer::Shutdown();
    }

    void EngineApp::PushLayer(Layer* layer) {

        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }


    void EngineApp::PushOverlay(Layer* layer) {

        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }


    void EngineApp::Start() {

        if (m_GameState == GameState::EDITOR || m_GameState == GameState::RUNTIME_EDITOR) {
            m_GuiLayer = new ImGuiLayer("#version 460");
            PushOverlay(m_GuiLayer);
        }
    }

    void EngineApp::Update() {
     
        Timestep ts = Timestep();

        while (m_GameState == GameState::PLAY || m_GameState == GameState::EDITOR || m_GameState == GameState::RUNTIME_EDITOR)
        {
            Run(ts);
        }
    }

    void EngineApp::Run(Timestep ts) {

        ts.Start();
    
        // Reset Event Subscriptions
        //EventHandler::Get()->Reset();
        
        // Test
        //EventHandler::Get()->SubscribeToEvent<KeyDownEvent>(InputEvent::Get(), &InputEvent::OnKeyDown);

        // if(!_minimized) { UISystem::StartProcessInputs(); }
        OnEvent();
        // if(!_minimized) { UISystem::EndProcessInputs(); }


        if(!m_Minimized) 
        {
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(ts);

            if (m_GameState == GameState::EDITOR || m_GameState == GameState::RUNTIME_EDITOR) {

                m_GuiLayer->Begin();

                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();

                m_GuiLayer->End();
            }
        }

        m_Window->OnUpdate();

        ts.Reset();
    }


    void EngineApp::Close() {
        
        m_GameState = GameState::EXIT;
    }


    void EngineApp::OnEvent() {

		// EventDispatcher dispatcher(e);
		// dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(EngineApp::OnWindowClose));
		// dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(EngineApp::OnWindowResize));

		// for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		// {
		// 	if (e.Handled) 
		// 		break;
		// 	(*it)->OnEvent(e);
		// }
    }
  

    // bool EngineApp::OnWindowClose(WindowCloseEvent& e)
	// {
	// 	m_GameState = GameState::EXIT;
	// 	return true;
	// }


	// bool EngineApp::OnWindowResize(WindowResizeEvent& e)
	// {
	// 	if (e.GetWidth() == 0 || e.GetHeight() == 0)
	// 	{
	// 		m_Minimized = true;
	// 		return false;
	// 	}

	// 	m_Minimized = false;
	// 	Render::OnWindowResize(e.GetWidth(), e.GetHeight());

	// 	return false;
	// }


    // void Application::SubmitToMainThread(const std::function<void()>& function)
	// {
	// 	std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

	// 	m_MainThreadQueue.emplace_back(function);
	// }


    // void EngineApp::ExecuteMainThreadQueue()
	// {
	// 	std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

	// 	for (auto& func : m_MainThreadQueue)
	// 		func();

	// 	m_MainThreadQueue.clear();
	// }
}