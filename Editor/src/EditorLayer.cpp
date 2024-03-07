#include "EditorLayer.h"


namespace Cober {

	//extern const std::filesystem::path _AssetPath;
	Editor::Editor() : Layer("Editor")
	{
		//_editorCamera = CreateUnique<EditorCamera>(45.0f, 1.778f, 0.01f, 1000.0f);

		/*new ViewportPanel();
		new SceneHierarchyPanel();
		new ContentBrowserPanel();
		new DataPanel();
		new MenuPanel();
		*/
	}

	void Editor::OnAttach() 
	{

	}

	void Editor::OnDetach() 
	{
		
	}

	void Editor::OnUpdate(Unique<Timestep>& ts) 
	{
		
	}

	void Editor::OnImGuiRender() 
	{
		InitDockspace();

		/* PANELS
		Entity& hoveredEntity = _activeScene->GetHoveredEntity();
		SceneHierarchyPanel::Get().OnGuiRender(hoveredEntity);
		ContentBrowserPanel::Get().OnGuiRender();
		ViewportPanel::Get().OnGuiRender(_editorCamera, _activeScene, hoveredEntity);
		DataPanel::Get().OnGuiRender(Engine::Get().GetGameMode(), hoveredEntity);
		MenuPanel::Get().OnGuiRender(_editorCamera, _activeScene, _editorScene, Engine::Get().GetGameMode(), Engine::Get().GetDebugMode());

		ViewportPanel::Get().PlayButtonBar(_editorScene, _activeScene, Engine::Get().GetGameState());
		*/

		EndDockspace();
	}

	void Editor::InitDockspace() 
	{
		// [[----- Init variables & dockspace -----]] (from Cherno)
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
			//window_flags |= ImGuiWindowFlags_NoTitleBar |  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiDragDropFlags_SourceAllowNullID;
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
}