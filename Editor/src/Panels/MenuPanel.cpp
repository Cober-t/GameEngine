#include <pch.h>

#include "Panels/MenuPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Render/Render2D.h"
#include "Core/EngineApp.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Cober {

	MenuPanel* MenuPanel::s_Instance = nullptr;

	MenuPanel::MenuPanel() 
	{
		s_Instance = this;
		m_Vsync = EngineApp::Get().GetWindow().GetVsync();
		m_FileBrowser = new ImGui::FileBrowser();
	}

	MenuPanel::~MenuPanel() 
	{
		delete s_Instance;
		s_Instance = nullptr;
	}


	void MenuPanel::OnGuiRender(Ref<EditorCamera>& editorCamera, Ref<Scene>& activeScene, Ref<Scene>& editorScene, Entity& hoveredEntity) 
	{
		m_FileBrowser->Display();

		if (m_FileBrowser->HasSelected()) 
		{
			switch(m_MenuFileOption)
			{
				case MenuOptions::SAVE:
					m_SaveFile = m_FileBrowser->GetSelected();
					LOG_INFO(m_SaveFile.filename().string());
					if (Scene::Save(activeScene, m_SaveFile.filename().string()) == false)
						m_SaveFile.clear();
					break;
				case MenuOptions::LOAD:
					m_LoadFile = m_FileBrowser->GetSelected();

					if (m_LoadFile.string().rfind(".lua") != std::string::npos)
					{
						editorScene = Scene::Load(m_LoadFile.filename().string());
						if (editorScene)
						{
							m_SaveFile = m_LoadFile;
							hoveredEntity = Entity();
							activeScene = editorScene;
							EngineApp::Get().SetGameState(GameState::EDITOR);
							SceneHierarchyPanel::Get().SetContext(activeScene);
						}
					}
					break;
				case MenuOptions::COMPILE:
					m_OutputCompilePath = m_FileBrowser->GetSelected();
					break;
			}
			m_FileBrowser->ClearSelected();
		}

		if (ImGui::BeginMenuBar()) 
		{
			if (ImGui::BeginMenu(ICON_FA_FILE  "  File")) 
			{
				if (ImGui::MenuItem(ICON_FA_DOWNLOAD "  Save Scene As..."))
				{
					m_MenuFileOption = MenuOptions::SAVE;
					m_FileBrowser = new ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
					m_FileBrowser->Open();
				}
				if (ImGui::MenuItem(ICON_FA_DOWNLOAD "  Save Scene"))
				{
					m_MenuFileOption = MenuOptions::SAVE;
					if (m_SaveFile.empty())
					{
						m_FileBrowser = new ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
						m_FileBrowser->Open();
					}
				}
				if (ImGui::MenuItem(ICON_FA_UPLOAD "  Load Scene")) 
				{
					m_FileBrowser->Open();
					m_MenuFileOption = MenuOptions::LOAD;
				}

				if (ImGui::MenuItem(ICON_FA_TIMES "  Exit"))
					EngineApp::Get().Close();

				if (ImGui::Checkbox("2D", &editorCamera->IsPerspective()))
                {
                    // TODO: Change to 2D projection
					// editorCamera->UpdateProjection(game2D);
                }

				if (ImGui::Checkbox("Fullscreen", &m_Fullscreen))
					EngineApp::Get().GetWindow().ChangeFullScreen();
				
				if (ImGui::Checkbox("VSYNC", &m_Vsync))
					EngineApp::Get().GetWindow().SetVSync(m_Vsync);


				ImGui::SetNextItemWidth(120.0f);
				if (ImGui::BeginCombo("##", m_CurrentBuildOption)) 
				{
					for (int n = 0; n < IM_ARRAYSIZE(m_BuildValues); n++) 
					{
						bool selected = (m_CurrentBuildOption == m_BuildValues[n]);
						if (ImGui::Selectable(m_BuildValues[n], selected)) 
						{
							m_FileBrowser = new ImGui::FileBrowser(ImGuiFileBrowserFlags_SelectDirectory);
							m_FileBrowser->Open();
							m_CurrentBuildOption = m_BuildValues[n];
							m_MenuFileOption = MenuOptions::COMPILE;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::SameLine();

				if (ImGui::MenuItem("Compile"))
				{
					if (!m_OutputCompilePath.empty())
						LOG_INFO(m_OutputCompilePath)
					// Compile Command
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu(ICON_FA_COG "  Options")) 
            {
				ImGui::SetNextItemWidth(120.0f);
				if (ImGui::InputFloat("Line Thickness", &Render2D::GetStats().LineThickness))
				{
					RenderGlobals::SetLineWidth(Render2D::GetStats().LineThickness);
				}

				ImGui::SetNextItemWidth(120.0f);
				if (ImGui::BeginCombo(ICON_FA_TELEVISION  "  Resolution", m_CurrentScreenSize)) 
                {
					for (int i = 0; i < SCREEN_SIZE::N_RESOLUTIONS; i++) 
                    {
						bool isSelected = m_CurrentScreenSize == m_ScreenValues[i];
						if (ImGui::Selectable(m_ScreenValues[i], isSelected)) 
                        {
							m_CurrentScreenSize = m_ScreenValues[i];
							if (isSelected)
								Resize(editorCamera, m_VPSize[i].x, m_VPSize[i].y, editorCamera->IsPerspective());
						}
					}
					ImGui::EndCombo();
				}

				if (ImGui::Checkbox(ICON_FA_CODE  "  Debug Mode", &EngineApp::Get().IsDebugMode()))
				{
					Debug2DPhysics::Get().UpdateFlags();
				}

				if (EngineApp::Get().IsDebugMode())
				{
					if (ImGui::Checkbox("Shapes", &Debug2DPhysics::Settings().shapes) ||
						ImGui::Checkbox("Joints", &Debug2DPhysics::Settings().joints) ||
						ImGui::Checkbox("AABBs", &Debug2DPhysics::Settings().aabbs) ||
						ImGui::Checkbox("Pair", &Debug2DPhysics::Settings().pair) ||
						ImGui::Checkbox("Center of Mass", &Debug2DPhysics::Settings().center_of_mass))
					{
							Debug2DPhysics::Get().UpdateFlags();
					}
				}
				
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


	void MenuPanel::Resize(Ref<EditorCamera>& camera, int width, int height, bool ortho) 
    {
        // Todo: Change between ortho and perspective projection
		ViewportPanel::Get().ResizeFramebufferSpecification(camera, width, height);
	}
}