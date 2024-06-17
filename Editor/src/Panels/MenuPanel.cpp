#include <pch.h>

#include "Panels/MenuPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Render/Render2D.h"
#include "Core/EngineApp.h"
// #include "Render/Text/Font.h"

#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Cober {

	static Ref<Font> s_Font;
	MenuPanel* MenuPanel::s_Instance = nullptr;

	MenuPanel::MenuPanel() 
	{
		s_Instance = this;
		s_Font = Font::GetDefault();
		m_ShotFontAtlas = false;
		// m_FileBrowser = new ImGui::FileBrowser();
		m_Settings.Vsync = EngineApp::Get().GetWindow().GetVsync();

		std::filesystem::path solutionDirPath = SOLUTION_DIR;
		m_BuildFileCommand = solutionDirPath / "setup" / "Compile.bat";
		m_CompileFileCommand = std::filesystem::current_path() / "ScriptModule" / "build.bat";

		std::string exeName = EngineApp::Get().GetSpecification().Name + ".exe";
		m_GamePath = std::filesystem::current_path() / "build" / "Debug-windows-x86_64" / exeName;

		m_ScenesPath = std::filesystem::current_path() / "assets" / "scenes";
	}

	MenuPanel::~MenuPanel() 
	{
		delete s_Instance;
		s_Instance = nullptr;
	}


	void MenuPanel::OnGuiRender(Ref<EditorCamera>& editorCamera) 
	{
		// m_FileBrowser->Display();

		// if (m_FileBrowser->HasSelected()) 
		// {
		// 	OpenFileDialog();
		// }

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
			}
			if (ImGui::MenuItem(ICON_FA_DOWNLOAD "  Save Scene"))
			{
				Scene::Save(Editor::GetActiveScene(), Editor::GetActiveScene()->GetName());
			}


			const char* sceneHandler = Editor::GetActiveScene()->GetName().c_str();
			if (ImGui::BeginCombo(ICON_FA_UPLOAD "  Load Scene", Editor::GetActiveScene()->GetName().c_str()))
			{
				std::vector<std::filesystem::path> scenes;
				for (const auto & entry : std::filesystem::directory_iterator(m_ScenesPath))
					scenes.push_back(entry.path());
					
				for (int i = 0; i < scenes.size(); i++)
				{
					bool isSelected = sceneHandler == scenes[i].filename().string().c_str();
					if (ImGui::Selectable(scenes[i].filename().string().c_str(), isSelected)) 
					{
						sceneHandler = scenes[i].filename().string().c_str();
						Editor::SetEditorScene(Scene::Load(scenes[i].filename().string()));
						if (Editor::SetEditorScene)
						{
							Editor::SelectedEntity();
							Editor::SetActiveScene(Editor::GetEditorScene());
							EngineApp::Get().SetGameState(EngineApp::GameState::EDITOR);
							SceneHierarchyPanel::Get().SetContext(Editor::GetActiveScene());
						}
					}
				}

				ImGui::EndCombo();
			}

			if (ImGui::MenuItem(ICON_FA_TIMES "  Exit"))
				EngineApp::Get().Close();


			if (ImGui::Checkbox("Fullscreen", &m_Settings.Fullscreen))
				EngineApp::Get().GetWindow().ChangeFullScreen();
			
			if (ImGui::Checkbox("VSYNC", &m_Settings.Vsync))
				EngineApp::Get().GetWindow().SetVSync(m_Settings.Vsync);

			if (ImGui::MenuItem("Build Game"))
			{
				std::string command = m_BuildFileCommand.string() + " game " + "\"" + std::filesystem::current_path().string() + "\"";
				system(command.c_str());
				LOG_CORE_INFO("Build command: {0}", command.c_str());
				if (std::filesystem::exists(m_CompileFileCommand))
				{
					system(m_CompileFileCommand.string().c_str());
					LOG_CORE_INFO("Compile command: {0}", m_CompileFileCommand.string().c_str());
				}

			}
			if (ImGui::MenuItem("Compile Scripts"))
			{
				if (std::filesystem::exists(m_CompileFileCommand))
				{
					system(m_CompileFileCommand.string().c_str());
					LOG_CORE_INFO("Compile command: {0}", m_CompileFileCommand.string().c_str());
				}
			}

			if (ImGui::MenuItem("Play Game"))
			{
				if (std::filesystem::exists(m_GamePath))
				{
					system(m_GamePath.string().c_str());
					LOG_CORE_INFO("Compile command: {0}", m_GamePath.string().c_str());
				}
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
			if (ImGui::Checkbox(ICON_FA_CAMERA  "  Camera Perspective", &editorCamera->IsPerspective()))
			{
				if (editorCamera->IsMainCamera())
				{
					if (editorCamera->IsPerspective() == false)
					{
						editorCamera->GetSettings().focalPoint = glm::vec3(0.0f, 0.0f, -1.0f);
						editorCamera->GetSettings().yaw = 0.0f;
						editorCamera->GetSettings().pitch = 0.0;
						editorCamera->GetSettings().roll = 0.0f;
					}
						
					ViewportPanel::Get().MustResize();
				}
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
			
			ImGui::Checkbox(ICON_FA_FILE_TEXT_O  "  Font Atlas", &m_ShotFontAtlas);
			if (m_ShotFontAtlas)
				ImGui::Image((ImTextureID)s_Font->GetAtlasTexture()->GetRendererID(), { 512,512 }, {0, 1}, {1, 0});
			
			ImGui::EndMenu();
		}
	}


	void MenuPanel::OpenFileDialog() 
    {
		// switch(m_MenuFileOption)
		// {
		// 	case MenuOptions::SAVE:
		// 		m_SaveFile = m_FileBrowser->GetSelected();
		// 		LOG_INFO(m_SaveFile.filename().string());
		// 		if (Scene::Save(Editor::GetActiveScene(), m_SaveFile.filename().string()) == false)
		// 			m_SaveFile.clear();
		// 		break;

		// 	case MenuOptions::LOAD:
		// 		m_LoadFile = m_FileBrowser->GetSelected();

		// 		if (m_LoadFile.string().rfind(".lua") != std::string::npos)
		// 		{
		// 			Editor::SetEditorScene(Scene::Load(m_LoadFile.filename().string()));
		// 			if (Editor::GetEditorScene())
		// 			{
		// 				m_SaveFile = m_LoadFile;
		// 				Editor::SelectedEntity();
		// 				Editor::SetActiveScene(Editor::GetEditorScene());
		// 				EngineApp::Get().SetGameState(EngineApp::GameState::EDITOR);
		// 				SceneHierarchyPanel::Get().SetContext(Editor::GetActiveScene());
		// 			}
		// 		}
		// 		break;
		// }
		// m_FileBrowser->ClearSelected();
	}
}