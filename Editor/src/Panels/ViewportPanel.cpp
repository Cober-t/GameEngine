#include "EditorLayer.h"
#include "Panels/ViewportPanel.h"
#include "Panels/DataPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Core/Utils.h"

#include "ImGuizmo/ImGuizmo.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>

namespace Cober {

	ViewportPanel* ViewportPanel::m_Instance = nullptr;

	ViewportPanel::ViewportPanel() 
	{
		m_Instance = this;

		m_AssetIconMap["play"] = EditorResources::PlayIcon;
		m_AssetIconMap["stop"] = EditorResources::StopIcon;
	}


	ViewportPanel::~ViewportPanel() 
	{
		delete m_Instance;
		m_Instance = nullptr;
	}


	// void ViewportPanel::RenderSkybox() 
    // {
	// 	EditorCamera::RenderSkybox(MenuPanel::Get().ColorSelected());
	// }


	void ViewportPanel::CreateFramebuffer(uint32_t width, uint32_t height) 
    {
		m_Fbo = Framebuffer::Create(width, height);
	}


	void ViewportPanel::BindFramebuffer() 
    {
		m_Fbo->Bind();  
	}


	void ViewportPanel::UnbindFramebuffer()
    {
		m_Fbo->Unbind();
	}


	void ViewportPanel::SetCursorEntity() 
    {
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_MinViewportBound.x;
		my -= m_MinViewportBound.y;
		glm::vec2 viewportSize = glm::vec2(m_MaxViewportBound.x - m_MinViewportBound.x, m_MaxViewportBound.y - m_MinViewportBound.y);
		my = viewportSize.y - my;
		int mouseX = (int)mx - m_ViewportMargin.x;
		int mouseY = (int)my - m_ViewportMargin.y;

		DataPanel::Get().SetMouseX(mouseX);
		DataPanel::Get().SetMouseY(mouseY);

		if (mouseX >= 0 && mouseY >= 0 && mouseX < ((int)m_ViewportSize.x) && mouseY < ((int)m_ViewportSize.y))
		{
			int pixelData = m_Fbo->ReadPixel(1, mouseX, mouseY);

			if (ImGui::IsMouseClicked(0) && !ImGuizmo::IsUsing())
			{
				if (pixelData == -1)
					Editor::SetSelectedEntity();
				else
					Editor::SetSelectedEntity(Entity((entt::entity)pixelData, Editor::GetActiveScene().get()));

				if (!Input::IsKeyDown(KeyCode::LeftAlt))
				{
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;	
					SceneHierarchyPanel::Get().SetSelectedEntity(Editor::SelectedEntity());
				}
			}
		}
	}


	void ViewportPanel::OnEvent(Event& event) 
    {
		// Gizmos
		if (!ImGuizmo::IsUsing())
		{
			if (Input::IsKeyDown(KeyCode::Q))
				m_GizmoType = -1;
			if (Input::IsKeyDown(KeyCode::W))
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			if (Input::IsKeyDown(KeyCode::E))
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			if (Input::IsKeyDown(KeyCode::R))
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
		}
	}


	void ViewportPanel::ResizeViewport(Ref<Camera> camera) 
    {
		FramebufferSpecification spec = m_Fbo->GetSpecification();
		if (m_MustResize || m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != (uint32_t)m_ViewportSize.x || spec.Height != (uint32_t)m_ViewportSize.y))
		{
			m_MustResize = false;
			ResizeFramebufferSpecification(camera, (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
	}

	void ViewportPanel::ResizeFramebufferSpecification(Ref<Camera> camera, uint32_t width, uint32_t height)
	{
		// FIS THIS!! Performance issue
		m_Fbo->Resize(width, height);
		camera->SetViewportSize(width, height);
	}


	void ViewportPanel::OnGuiRender(Ref<EditorCamera>& editorCamera, Ref<Camera>& camera)
    {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		// Hovered Entity
		auto viewportPanelSize = ImGui::GetContentRegionAvail();
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		
		m_MinViewportBound = { viewportMinRegion.x + viewportOffset.x,
							  viewportMinRegion.y + viewportOffset.y };
		m_MaxViewportBound = { viewportMaxRegion.x + viewportOffset.x, 
							  viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		EngineApp::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

		if (dynamic_cast<GameCamera*>(Editor::GetActiveCamera().get()) != nullptr)
		{
			float screenWidth  = EngineApp::Get().GetWindow().GetWidth();
			float screenHeight = EngineApp::Get().GetWindow().GetHeight();

			viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			
			if (viewportPanelSize.x >= viewportPanelSize.y)
				m_ViewportSize = { m_ViewportSize.y * screenWidth / screenHeight  , m_ViewportSize.y };
			else if (viewportPanelSize.x < viewportPanelSize.y)
				m_ViewportSize = { m_ViewportSize.x, m_ViewportSize.x * screenHeight / screenWidth };
			
			if (m_ViewportSize.x >= viewportPanelSize.x) 
			{
				m_ViewportSize.x = viewportPanelSize.x;
				m_ViewportSize.y = m_ViewportSize.x * screenHeight / screenWidth;
				m_ViewportMargin.y = (viewportPanelSize.y - m_ViewportSize.y) / 2;
				m_ViewportMargin.x = 0.0f;
			}

			if (m_ViewportSize.y >= viewportPanelSize.y) 
			{
				m_ViewportSize.y = viewportPanelSize.y;
				m_ViewportSize.x = m_ViewportSize.y * screenWidth / screenHeight;
				m_ViewportMargin.x = (viewportPanelSize.x - m_ViewportSize.x) / 2;
				m_ViewportMargin.y = 0.0f;
			}
		}
		else
		{
			viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_ViewportMargin = { 0.0f, 0.0f };
		}

		/////////////////////////
		// Center Viewport Image
		ImVec2 contentRegionSize{ (viewportPanelSize.x - m_ViewportSize.x) * 0.5f,
								  (viewportPanelSize.y - m_ViewportSize.y) * 0.5f };
		ImGui::SetCursorPos(contentRegionSize);

		uint32_t textureID = m_Fbo->GetColorAttachmentRenderID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		/////////////////////////
		// Enable Camera Controls
		m_AllowViewportCameraEvents = (ImGui::IsMouseHoveringRect(m_MinViewportBound, m_MaxViewportBound) && m_ViewportFocused) || m_StartedCameraClickInViewport;

		if (((Input::IsKeyDown(KeyCode::LeftAlt) && (Input::IsMouseButtonDown(MouseButton::Left) || (Input::IsMouseButtonDown(MouseButton::Middle)))) || Input::IsMouseButtonDown(MouseButton::Right)) && !m_StartedCameraClickInViewport && m_ViewportFocused && m_ViewportHovered)
			m_StartedCameraClickInViewport = true;

		if (!Input::IsMouseButtonDown(MouseButton::Right) && !(Input::IsKeyDown(KeyCode::LeftAlt) && (Input::IsMouseButtonDown(MouseButton::Left) || (Input::IsMouseButtonDown(MouseButton::Middle)))))
			m_StartedCameraClickInViewport = false;


		///////////////////////////////////
		// Export to DragDropViewportTarget
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) 
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				m_FilePath = (std::filesystem::current_path() / "assets" / path).string();
				LOG_INFO(m_FilePath);
				
				//Textures
				if (Editor::SelectedEntity() && Editor::SelectedEntity().HasComponent<Render2DComponent>()) 
				{
					auto lastDot = m_FilePath.find_last_of('.');
					std::string format = lastDot != std::string::npos ? m_FilePath.substr(lastDot) : "null";
					if (lastDot != std::string::npos && (format == ".png" || format == ".jpg" || format == ".jpeg"))
					{
						auto textureHolder = Texture::Create(m_FilePath);
						Editor::SelectedEntity().GetComponent<Render2DComponent>().texture = textureHolder;
						Editor::SelectedEntity().GetComponent<TransformComponent>().scale.x = textureHolder->GetWidth()*0.01;
						Editor::SelectedEntity().GetComponent<TransformComponent>().scale.y = textureHolder->GetHeight()*0.01;
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		///////////////////////////////
		//Gizmos
		if (Editor::SelectedEntity() && 
			m_GizmoType != -1 		 && 
			dynamic_cast<EditorCamera*>(Editor::GetActiveCamera().get()) != nullptr) // Camera type must be "EditorCamera"
		{
			ImGuizmo::SetOrthographic(!editorCamera->IsPerspective());
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_MinViewportBound.x, m_MinViewportBound.y, 
								m_MaxViewportBound.x - m_MinViewportBound.x, 
								m_MaxViewportBound.y - m_MinViewportBound.y);

			glm::mat4 cameraView = editorCamera->GetViewMatrix();
			const glm::mat4& cameraProjection = editorCamera->GetProjectionMatrix();

			auto& tc = Editor::SelectedEntity().GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// bool snap = MenuPanel::Get().Snap();
			// float snapValue = MenuPanel::Get().GetSnapValue() / 10; // Snap to 0.5m for translation/scale
			bool snap = false;
			float snapValue = 0.5f;
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::WORLD, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			
			if (ImGuizmo::IsUsing() && !Input::IsKeyDown(KeyCode::LeftAlt))
			{
				glm::vec3 translation, rotation, scale;
				Utils::DecomposeTransform(transform, translation, rotation, scale);

				// if (Engine::Get().GetGameMode())
				// translation.z = tc.position.z;
				
				glm::vec3 deltaRotation = rotation - tc.rotation;

				tc.position = translation;
				tc.rotation += deltaRotation;
				tc.scale = scale;
			}
		}

		ImGui::PopStyleVar();
		ImGui::End();


		// ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		// ImGui::Begin("Game");

		// // START CONSTRAIN VIEWPORT SCENE
		// float screenWidth  = EngineApp::Get().GetWindow().GetWidth();
		// float screenHeight = EngineApp::Get().GetWindow().GetHeight();
		
		// if (viewportPanelSize.x >= viewportPanelSize.y)
		// 	m_ViewportSize = { m_ViewportSize.y * screenWidth / screenHeight  , m_ViewportSize.y };
		// else if (viewportPanelSize.x < viewportPanelSize.y)
		// 	m_ViewportSize = { m_ViewportSize.x, m_ViewportSize.x * screenHeight / screenWidth };
		
		// if (m_ViewportSize.x >= viewportPanelSize.x) 
		// {
		// 	m_ViewportSize.x = viewportPanelSize.x;
		// 	m_ViewportSize.y = m_ViewportSize.x * screenHeight / screenWidth;
		// 	m_ViewportMargin.y = (viewportPanelSize.y - m_ViewportSize.y) / 2;
		// 	m_ViewportMargin.x = 0.0f;
		// }

		// if (m_ViewportSize.y >= viewportPanelSize.y) 
		// {
		// 	m_ViewportSize.y = viewportPanelSize.y;
		// 	m_ViewportSize.x = m_ViewportSize.y * screenWidth / screenHeight;
		// 	m_ViewportMargin.x = (viewportPanelSize.x - m_ViewportSize.x) / 2;
		// 	m_ViewportMargin.y = 0.0f;
		// }
		// // END CONSTRAIN VIEWPORT SCENE

		// /////////////////////////
		// // Center Viewport Image
		// ImVec2 gameContentRegionSize{ (viewportPanelSize.x - m_ViewportSize.x) * 0.5f,
		// 						  (viewportPanelSize.y - m_ViewportSize.y) * 0.5f };
		// ImGui::SetCursorPos(gameContentRegionSize);

		// uint32_t textureGameID = m_Fbo->GetColorAttachmentRenderID();
		// ImGui::Image(reinterpret_cast<void*>(textureGameID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		
		// ImGui::PopStyleVar();
		// ImGui::End();
	}


	void ViewportPanel::PlayButtonBar(EngineApp::GameState gameState) 
    {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		auto icon = gameState == EngineApp::GameState::EDITOR ? ICON_FA_PLAY : ICON_FA_STOP;
		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		if (ImGui::Button(icon)) 
        {
			m_GizmoType = -1;
			if (gameState == EngineApp::GameState::EDITOR) 
            {
				Log::ClearLogMessages();	
				EngineApp::Get().SetGameState(EngineApp::GameState::RUNTIME_EDITOR);

				Editor::SetActiveScene(Scene::Copy(Editor::GetEditorScene()));
				Editor::GetActiveScene()->OnSimulationStart();
				SceneHierarchyPanel::Get().SetContext(Editor::GetActiveScene());
			}
			else if (gameState == EngineApp::GameState::RUNTIME_EDITOR) 
            {
				Editor::GetActiveScene()->OnSimulationStop();

				EngineApp::Get().SetGameState(EngineApp::GameState::EDITOR);

				Editor::SetActiveScene(Editor::GetEditorScene());
				SceneHierarchyPanel::Get().SetContext(Editor::GetActiveScene());

				// Provisional fix to avoid crash
				NativeScriptFn::FreeScriptLibrary();
				Editor::SetSelectedEntity();
			}
		}

		if (gameState == EngineApp::GameState::RUNTIME_EDITOR)
		{
			ImGui::SameLine();

			if (ImGui::Button(ICON_FA_PAUSE))
				Editor::GetActiveScene()->Pause();

			ImGui::SameLine();

			if (ImGui::Button(ICON_FA_STEP_FORWARD))
		
				Editor::GetActiveScene()->Step();
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}
}