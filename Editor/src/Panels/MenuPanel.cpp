#include <pch.h>

#include "Panels/MenuPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Render/Render2D.h"
#include "Core/EngineApp.h"
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Cober {

	MenuPanel* MenuPanel::s_Instance = nullptr;

	MenuPanel::MenuPanel() 
	{
		s_Instance = this;
		m_FileBrowser = new ImGui::FileBrowser();
		m_Settings.Vsync = EngineApp::Get().GetWindow().GetVsync();
	}

	MenuPanel::~MenuPanel() 
	{
		delete s_Instance;
		s_Instance = nullptr;
	}


	void MenuPanel::OnGuiRender(Ref<EditorCamera>& editorCamera) 
	{
		m_FileBrowser->Display();

		if (m_FileBrowser->HasSelected()) 
		{
			OpenFileDialog();
		}

		if (ImGui::BeginMenuBar()) 
		{

			FileMenu(editorCamera);

			OptionsMenu(editorCamera);

			ImGui::EndMenuBar();
		}
	}


	void MenuPanel::FileMenu(Ref<EditorCamera>& editorCamera)
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
				else
				{
					Scene::Save(Editor::GetActiveScene(), m_SaveFile.filename().string());
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

			if (ImGui::Checkbox("Fullscreen", &m_Settings.Fullscreen))
				EngineApp::Get().GetWindow().ChangeFullScreen();
			
			if (ImGui::Checkbox("VSYNC", &m_Settings.Vsync))
				EngineApp::Get().GetWindow().SetVSync(m_Settings.Vsync);

			if (ImGui::MenuItem("Compile"))
			{
				std::filesystem::path solutionDirPath = SOLUTION_DIR;
				std::filesystem::path compileCommand = solutionDirPath / "setup" / "Compile.bat";
				std::string command = compileCommand.string() + " game " + "\"" + std::filesystem::current_path().string() + "\"";
				system(command.c_str());
				LOG_CORE_INFO("Build command: {0}", command.c_str());
			}

			ImGui::EndMenu();
		}
	}


	void MenuPanel::OptionsMenu(Ref<EditorCamera>& editorCamera)
	{
		if (ImGui::BeginMenu(ICON_FA_COG "  Options")) 
		{
			ImGui::SetNextItemWidth(120.0f);
			if (ImGui::InputFloat("Line Thickness", &Render2D::GetStats().LineThickness))
			{
				RenderGlobals::SetLineWidth(Render2D::GetStats().LineThickness);
			}

			ImGui::SetNextItemWidth(120.0f);
			ImGui::InputFloat("Physics Timestep", &Physics2D::GetSettings().TimeStep);

			ImGui::SetNextItemWidth(120.0f);
			ImGui::InputFloat("Physics Gravity", &Physics2D::GetSettings().Gravity);

			ImGui::SetNextItemWidth(120.0f);
			ImGui::InputInt("Physics Velocity Iterations", &Physics2D::GetSettings().VelocityIterations);

			ImGui::SetNextItemWidth(120.0f);
			ImGui::InputInt("Physics Position Iterations", &Physics2D::GetSettings().PositionIterations);

			ImGui::SetNextItemWidth(120.0f);
			if (ImGui::BeginCombo(ICON_FA_TELEVISION  "  Resolution", m_Settings.CurrentScreenSize)) 
			{
				for (int i = 0; i < ScreenSize::N_RESOLUTIONS; i++) 
				{
					bool isSelected = m_Settings.CurrentScreenSize == m_Settings.ScreenValues[i];
					if (ImGui::Selectable(m_Settings.ScreenValues[i], isSelected)) 
					{
						m_Settings.CurrentScreenSize = m_Settings.ScreenValues[i];
						if (isSelected)
							Resize(editorCamera, m_Settings.VPSize[i].x, m_Settings.VPSize[i].y, editorCamera->IsPerspective());
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
	}


	void MenuPanel::OpenFileDialog() 
    {
		switch(m_MenuFileOption)
		{
			case MenuOptions::SAVE:
				m_SaveFile = m_FileBrowser->GetSelected();
				LOG_INFO(m_SaveFile.filename().string());
				if (Scene::Save(Editor::GetActiveScene(), m_SaveFile.filename().string()) == false)
					m_SaveFile.clear();
				break;

			case MenuOptions::LOAD:
				m_LoadFile = m_FileBrowser->GetSelected();

				if (m_LoadFile.string().rfind(".lua") != std::string::npos)
				{
					Editor::SetEditorScene(Scene::Load(m_LoadFile.filename().string()));
					if (Editor::GetEditorScene())
					{
						m_SaveFile = m_LoadFile;
						Editor::SelectedEntity();
						Editor::SetActiveScene(Editor::GetEditorScene());
						EngineApp::Get().SetGameState(EngineApp::GameState::EDITOR);
						SceneHierarchyPanel::Get().SetContext(Editor::GetActiveScene());
					}
				}
				break;
		}
		m_FileBrowser->ClearSelected();
	}


	void MenuPanel::Resize(Ref<EditorCamera>& camera, int width, int height, bool ortho) 
    {
        // Todo: Change between ortho and perspective projection
		ViewportPanel::Get().ResizeFramebufferSpecification(camera, width, height);
	}
}