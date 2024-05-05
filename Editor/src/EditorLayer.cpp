#include "EditorLayer.h"


namespace Cober {

	// extern const std::filesystem::path _AssetPath;
	Editor::Editor() : Layer("Editor")
	{
		m_EditorCamera = CreateUnique<EditorCamera>(45.0f, EngineApp::Get().GetWindow().GetWidth(), EngineApp::Get().GetWindow().GetHeight(), 0.01f, 1000.0f);
		m_EditorCamera->SetActive(true);

		new ViewportPanel();
		new DataPanel();
		new ConsolePanel();
		new SceneHierarchyPanel();
		new ContentBrowserPanel();
		new MenuPanel();

		new Debug2DPhysics();
	}


	void Editor::OnAttach() 
	{
		m_ActiveScene = Scene::Load("Scene2.lua");
		m_ActiveScene->OnRuntimeStart();
		m_EditorScene = m_ActiveScene;

		Primitive::Grid::Init();

		ViewportPanel::Get().CreateFramebuffer(m_EditorCamera->m_ViewportWidth, m_EditorCamera->m_ViewportHeight);
		SceneHierarchyPanel::Get().SetContext(m_ActiveScene);
	}


	void Editor::OnDetach() 
	{
		m_ActiveScene->OnRuntimeStop();
		ViewportPanel::Get().UnbindFramebuffer();

		m_ActiveScene  = nullptr;
		m_EditorScene  = nullptr;
		m_EditorCamera = nullptr;

 		LOG_INFO("Detached Editor Layer!");
	}


	void Editor::OnUpdate(Unique<Timestep>& ts) 
	{
		bool projectMode = false;
		ViewportPanel::Get().ResizeViewport(m_EditorCamera, projectMode);
		ViewportPanel::Get().BindFramebuffer();
		// ViewportPanel::Get().RenderSkybox();

		RenderGlobals::SetClearColor(46, 47, 52);
		// RenderGlobals::SetClearColor(32, 167, 219);
		RenderGlobals::Clear();
		
		ViewportPanel::Get().FBOClearAttachments(1, -1);

		auto& colors = ImGui::GetStyle().Colors;

		switch (EngineApp::Get().GetGameState()) 
		{
			case GameState::EDITOR:
			{
				colors[ImGuiCol_WindowBg] = ImGui::ColorConvertU32ToFloat4(Colors::Theme::titlebar);
				m_EditorCamera->SetActive(ViewportPanel::Get().AllowViewportCameraEvents());
				m_EditorCamera->OnUpdate(ts);
				m_ActiveScene->OnUpdateRuntime(ts, m_EditorCamera);
				// Commented because of a problem with the framebuffer and camera depth
				// Primitive::Grid::Draw(m_EditorCamera);
				break;
			}
			case GameState::RUNTIME_EDITOR: 
			{
				colors[ImGuiCol_WindowBg] = ImVec4(0, 0.0, 0.0, 0.268f);
				m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
				break;
			}
		}

		ViewportPanel::Get().SetCursorEntity(m_ActiveScene, m_HoveredEntity);

		ViewportPanel::Get().UnbindFramebuffer();
	}


	void Editor::OnImGuiRender() 
	{
		InitDockspace();

		ViewportPanel::Get().OnGuiRender(m_EditorCamera, m_ActiveScene, m_HoveredEntity);
		DataPanel::Get().OnGuiRender(m_HoveredEntity);
		ConsolePanel::Get().OnImGuiRender();
		SceneHierarchyPanel::Get().OnGuiRender(m_HoveredEntity);
		ContentBrowserPanel::Get().OnGuiRender();
		
		MenuPanel::Get().OnGuiRender(m_EditorCamera, m_ActiveScene, m_EditorScene, m_HoveredEntity);

		ViewportPanel::Get().PlayButtonBar(m_EditorScene, m_ActiveScene, EngineApp::Get().GetGameState(), m_HoveredEntity);

		EndDockspace();
	}


	void Editor::InitDockspace() 
	{
		// [[----- Init variables & dockspace -----]]
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!dockspaceOpen)
			EngineApp::Get().Close();

		// [[----- BEGIN DOCKSPACE ----]]
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		EngineApp& app = EngineApp::Get();
		const char* title = app.GetWindow().GetTitle().c_str();
		ImGui::Begin(title, &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		//float minWinSizeY = style.WindowMinSize.y;
		style.WindowMinSize.x = 200.0f;
		//style.WindowMinSize.y = 25.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;
		//style.WindowMinSize.y = minWinSizeY;
	}


	void Editor::EndDockspace() 
	{
		ImGui::End();
	}


	void Editor::OnEvent(Event& event)
	{
		auto gameState = EngineApp::Get().GetGameState();

		if (gameState == GameState::EDITOR || gameState == GameState::RUNTIME_EDITOR)
		{
			if (m_AllowViewportCameraEvents)
				m_EditorCamera->OnEvent(event);
		}
		
		ViewportPanel::Get().OnEvent(event);
	}


	bool Editor::OnKeyPressed(KeyPressedEvent& event) 
	{
		return true;
	}
}