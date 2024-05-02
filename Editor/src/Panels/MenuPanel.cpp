#include <pch.h>

#include "Panels/MenuPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Render/Render2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Cober {

	MenuPanel* MenuPanel::s_Instance = nullptr;

	MenuPanel::MenuPanel() 
	{
		s_Instance = this;
	}

	MenuPanel::~MenuPanel() 
	{
		delete s_Instance;
		s_Instance = nullptr;
	}


	void MenuPanel::OnGuiRender(Ref<EditorCamera>& editorCamera, Ref<Scene>& activeScene, Ref<Scene>& editorScene, Entity& hoveredEntity, bool& game2D, bool& debugMode) 
	{
		m_FileBrowser.Display();
		if (m_FileBrowser.HasSelected()) {
			auto file_path = m_FileBrowser.GetSelected().string();
			m_FilePath = file_path;
			//_currentFile = file_path.substr(file_path.find_last_of("/\\") + 1);
			OpenFileDialog(activeScene, m_FilePath);

			m_FileBrowser.ClearSelected();
		}

		m_World2D = game2D;

		if (ImGui::BeginMenuBar()) 
		{
			if (ImGui::BeginMenu("File")) 
			{
				if(ImGui::MenuItem("Open File Explorer"))
					m_FileBrowser.Open();

				if (ImGui::MenuItem("Save Scene"))
					Scene::Save(activeScene, "Scene2.lua");	 // Test Scene

				if (ImGui::MenuItem("Load Scene")) 
				{
					hoveredEntity = Entity();
					editorScene = Scene::Load("Scene2.lua"); // Test Scene
					activeScene = editorScene;
					EngineApp::Get().SetGameState(GameState::EDITOR);
					SceneHierarchyPanel::Get().SetContext(activeScene);
					activeScene->OnRuntimeStart();
				}

				if (ImGui::MenuItem("Exit"))
					EngineApp::Get().Close();

				if (ImGui::Checkbox("2D", &game2D))
                {
                    // TODO: Change to 2D projection
					// editorCamera->UpdateProjection(game2D);
                }

				if (ImGui::Checkbox("Fullscreen", &m_Fullscreen))
					EngineApp::Get().GetWindow().ChangeFullScreen();


				if (ImGui::BeginCombo("Build Option", m_CurrentBuildOption)) 
				{
					for (int n = 0; n < IM_ARRAYSIZE(m_BuildValues); n++) 
					{
						bool selected = (m_CurrentBuildOption == m_BuildValues[n]);
						if (ImGui::Selectable(m_BuildValues[n], selected)) 
						{
							m_CurrentBuildOption = m_BuildValues[n];
							switch (n) {
							case BUILD_OPTION::WINDOWS:	/*Lod makefile path*/ break;
							case BUILD_OPTION::LINUX:	/*Lod makefile path*/ break;
							case BUILD_OPTION::WEB:		/*Lod makefile path*/ break;
							}
						}
					}
					ImGui::EndCombo();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options")) 
            {
				if (ImGui::BeginCombo("Resolution", m_CurrentScreenSize)) 
                {
					for (int i = 0; i < SCREEN_SIZE::N_RESOLUTIONS; i++) 
                    {
						bool isSelected = m_CurrentScreenSize == m_ScreenValues[i];
						if (ImGui::Selectable(m_ScreenValues[i], isSelected)) 
                        {
							m_CurrentScreenSize = m_ScreenValues[i];
							if (isSelected)
								Resize(editorCamera, activeScene, m_VPSize[i].x, m_VPSize[i].y, game2D);
						}
					}
					ImGui::EndCombo();
				}

				ImGui::Checkbox("Debug Mode", &debugMode);
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}


	void MenuPanel::OpenFileDialog(Ref<Scene>& activeScene, const std::filesystem::path& path) 
    {
		// if (m_MenuFileOption == MenuOptions::OPEN)
		// 	OpenFile(path);
		// else if (m_MenuFileOption == MenuOptions::SAVE_AS) 
        // {
		// 	SceneSerializer serializer(activeScene);
		// 	serializer.Serialize(path.string());
		// }
	}
	
	
	bool MenuPanel::MustSnap() 
	{
		return m_World2D == true ? m_Snap2D : m_Snap3D;
	}


	float MenuPanel::SnapValue() 
	{
		return m_World2D == true ? m_Snap2DValue : m_Snap3DValue;
	}


	void MenuPanel::Resize(Ref<EditorCamera>& camera, Ref<Scene>& scene, int width, int height, bool ortho) 
    {
        // Todo: Change between ortho and perspective projection
		ViewportPanel::Get().ResizeFramebufferSpecification(camera, width, height);
		camera->SetViewportSize(width, height);
	}
}