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

	MenuPanel* MenuPanel::s_Instance = nullptr;

	MenuPanel::MenuPanel() 
	{
		s_Instance = this;
		m_Reload = false;
		m_ShowDemoWindow = false;

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
		ManageSceneReloadingIfExists();

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
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			ImGui::PushItemWidth(130.0f);
			if (ImGui::InputText("##NewScene", buffer, sizeof(buffer)))
			{
				m_NewSceneName = (std::string)buffer;
			}
			ImGui::PopItemWidth();

			ImGui::SameLine();

			if (ImGui::Button(ICON_FA_FILE  "  New Scene"))
			{
				std::filesystem::path newScenePath = std::filesystem::current_path() / "assets" / "scenes";
    			std::ofstream ofs(newScenePath / m_NewSceneName);
			}

			const char* sceneHandler = Editor::GetActiveScene()->GetName().c_str();
			ImGui::PushItemWidth(150.0f);
			if (ImGui::BeginCombo(ICON_FA_DOWNLOAD, "  Save Scene As..."))
			{
				std::vector<std::filesystem::path> scenes;
				for (const auto & entry : std::filesystem::directory_iterator(m_ScenesPath))
					scenes.push_back(entry.path());

				for (int i = 0; i < scenes.size(); i++)
				{
					bool isSelected = sceneHandler == scenes[i].filename().string().c_str();
					if (ImGui::Selectable(scenes[i].filename().string().c_str(), isSelected)) 
					{
						Scene::Save(Editor::GetActiveScene(), scenes[i].filename().string());
					}
				}

				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			
			if (ImGui::MenuItem(ICON_FA_DOWNLOAD "  Save Scene"))
			{
				Scene::Save(Editor::GetActiveScene(), Editor::GetActiveScene()->GetName());
			}

			ImGui::PushItemWidth(150.0f);
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
						m_Reload = true;
						m_SceneToReload = scenes[i].filename().string();
						EngineApp::Get().SetGameState(EngineApp::GameState::EDITOR);
					}
				}

				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			

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
			ImGui::InputFloat("Physics Timestep", &physicsSettings.TimeStep);

			ImGui::SetNextItemWidth(120.0f);
			ImGui::InputFloat("Physics Gravity", &physicsSettings.Gravity);

			ImGui::SetNextItemWidth(120.0f);
			ImGui::InputInt("Physics Velocity Iterations", &physicsSettings.VelocityIterations);

			ImGui::SetNextItemWidth(120.0f);
			ImGui::InputInt("Physics Position Iterations", &physicsSettings.PositionIterations);

			ImGui::SetNextItemWidth(120.0f);
			if (ImGui::Checkbox(ICON_FA_CAMERA  "  Camera Perspective", &editorCamera->IsPerspective()))
			{
				GlobalCamera::perspective = (bool)editorCamera->IsPerspective();
				
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

			ImGui::Checkbox("Show Demo Window", &m_ShowDemoWindow);
			
			ImGui::EndMenu();
		}
		
		if (m_ShowDemoWindow)
			ImGui::ShowDemoWindow();
	}

	void MenuPanel::ManageSceneReloadingIfExists()
	{
		if (m_Reload)
		{
			m_Reload = false;
			Editor::SetEditorScene(Scene::Load(m_SceneToReload));
			Editor::SetActiveScene(Editor::GetEditorScene());
			Editor::SetSelectedEntity(Entity());
			SceneHierarchyPanel::Get().SetContext(Editor::GetActiveScene());

			Editor::GetActiveCamera()->SetPerspective(GlobalCamera::perspective);
			ViewportPanel::Get().MustResize();
		}
	}
}