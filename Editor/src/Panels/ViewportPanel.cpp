#include "Panels/ViewportPanel.h"
#include "Panels/DataPanel.h"
#include "Panels/SceneHierarchyPanel.h"
// #include "MenuPanel.h"

// #include "ImGuizmo/ImGuizmo.h"
// #include "Core/Utils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>

namespace Cober {

	ViewportPanel* ViewportPanel::m_Instance = nullptr;

	ViewportPanel::ViewportPanel() 
	{
		m_Instance = this;
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


	void ViewportPanel::SetCursorEntity(Ref<Scene>& activeScene, Entity& hoveredEntity) 
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
			hoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, activeScene.get());
			
		}
	}


	void ViewportPanel::OnEvent(Event& event, Entity& hoveredEntity) 
    {
		// Gizmos
		// 	if (Input::IsKeyDown(KeyCode::Q))
		// 	if (Input::IsKeyDown(KeyCode::W))
		// 	if (Input::IsKeyDown(KeyCode::E))
		// 	if (Input::IsKeyDown(KeyCode::R))

	}


	void ViewportPanel::ResizeViewport(Ref<EditorCamera> editorCamera, Ref<Scene>& activeScene, bool& game2D) 
    {
		FramebufferSpecification spec = m_Fbo->GetSpecification();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Fbo->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			editorCamera->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			// editorCamera->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y, game2D);
		}
	}


	void ViewportPanel::OnGuiRender(Ref<EditorCamera> editorCamera, Ref<Scene>& scene) 
    {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		//if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered() && !ImGuizmo::IsOver() && !Input::IsKeyDown(KeyCode::LeftAlt))
		//{
			//LOG_INFO("Mouse clicked imgui event!!");
			//scene->SetDefaultEntity(hoveredEntity);
			//SceneHierarchyPanel::Get().SetNullEntityContext();
		//}

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		// Hovered Entity
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

		// START CONSTRAIN VIEWPORT SCENE
		EngineApp& app = EngineApp::Get();
		float screenWidth  = app.GetWindow().GetWidth();
		float screenHeight = app.GetWindow().GetHeight();
		
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
		// END CONSTRAIN VIEWPORT SCENE

		// Center Viewport Image
		ImVec2 contentRegionSize{ (viewportPanelSize.x - m_ViewportSize.x) * 0.5f,
								  (viewportPanelSize.y - m_ViewportSize.y) * 0.5f };
		ImGui::SetCursorPos(contentRegionSize);

		m_AllowViewportCameraEvents = (ImGui::IsMouseHoveringRect(m_MinViewportBound, m_MaxViewportBound) && m_ViewportFocused) || m_StartedCameraClickInViewport;

		if (((Input::IsKeyDown(KeyCode::LeftAlt) && (Input::IsMouseButtonDown(MouseButton::Left) || (Input::IsMouseButtonDown(MouseButton::Middle)))) || Input::IsMouseButtonDown(MouseButton::Right)) && !m_StartedCameraClickInViewport && m_ViewportFocused && m_ViewportHovered)
			m_StartedCameraClickInViewport = true;

		if (!Input::IsMouseButtonDown(MouseButton::Right) && !(Input::IsKeyDown(KeyCode::LeftAlt) && (Input::IsMouseButtonDown(MouseButton::Left) || (Input::IsMouseButtonDown(MouseButton::Middle)))))
			m_StartedCameraClickInViewport = false;

		uint32_t textureID = m_Fbo->GetColorAttachmentRenderID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		///////////////////////////////////
		// Export to DragDropViewportTarget
		if (ImGui::BeginDragDropTarget()) 
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) 
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				m_FilePath = (std::filesystem::path(ASSETS_DIR) / path).string();
				LOG_INFO(m_FilePath);
				
				//Textures
				//if (hoveredEntity.GetIndex() != -1 && hoveredEntity.HasComponent<Sprite>()) {
					//auto lastDot = m_FilePath.find_last_of('.');
					//std::string format = lastDot != std::string::npos ? m_FilePath.substr(lastDot) : "null";
					//if (lastDot != std::string::npos && (format == ".png" || format == ".jpg" || format == ".jpeg"))
						//hoveredEntity.GetComponent<Sprite>().texture = Texture::Create(m_FilePath);
				//}
			}
			ImGui::EndDragDropTarget();
		}

		///////////////////////////////
		//Gizmos
		//Entity selectedEntity = SceneHierarchyPanel::Get().GetSelectedEntity();
		//if (selectedEntity.GetIndex() != -1 && selectedEntity.HasComponent<Sprite>() && m_GizmoType != -1)
		//{
		//	ImGuizmo::SetOrthographic(Engine::Get().GetGameMode());
		//	ImGuizmo::SetDrawlist();

		//	ImGuizmo::SetRect(m_MinViewportBound.x, m_MinViewportBound.y, 
		//					  m_MaxViewportBound.x - m_MinViewportBound.x, 
		//					  m_MaxViewportBound.y - m_MinViewportBound.y);
		//	
		//	// Runtime camera from entity
		//	// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
		//	// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
		//	// const glm::mat4& cameraProjection = camera.GetProjection();
		//	// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

		//	// Editor camera
		//	const glm::mat4& cameraProjection = editorCamera->GetProjection();
		//	glm::mat4 cameraView = editorCamera->GetView();

		//	// Entity transform
		//	auto& tc = selectedEntity.GetComponent<Transform>();
		//	glm::mat4 transform = tc.GetTransform();

		//	bool snap = MenuPanel::Get().MustSnap();
		//	float snapValue = MenuPanel::Get().SnapValue() / 10; // Snap to 0.5m for translation/scale
		//	// Snap to 45 degrees for rotation
		//	if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
		//		snapValue = 45.0f;

		//	float snapValues[3] = { snapValue, snapValue, snapValue };

		//	ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
		//		(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
		//		nullptr, snap ? snapValues : nullptr);

		//	if (ImGuizmo::IsUsing() && !SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT])
		//	{
		//		glm::vec3 translation, rotation, scale;
		//		Utils::DecomposeTransform(transform, translation, rotation, scale);

		//		if (Engine::Get().GetGameMode())
		//			translation.z = tc.position.z;
		//		
		//		glm::vec3 deltaRotation = rotation - tc.rotation;

		//		tc.position = translation;
		//		tc.rotation += deltaRotation;
		//		tc.scale = scale;
		//	}
		//}

		ImGui::PopStyleVar();
		ImGui::End();
	}


	void ViewportPanel::PlayButtonBar(Ref<Scene>& editorScene, Ref<Scene>& activeScene, GameState gameState) 
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

		float size = ImGui::GetWindowHeight() - 4.0f;
		const char* icon = gameState == GameState::EDITOR ? "I>" : "||";
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		if (ImGui::Button(icon, ImVec2(size, size))) 
        {
			if (gameState == GameState::EDITOR) 
            {
				EngineApp::Get().SetGameState(GameState::RUNTIME_EDITOR);
				activeScene = Scene::Copy(editorScene);
				SceneHierarchyPanel::Get().SetContext(activeScene);
				activeScene->OnSimulationStart();
			}
			else if (gameState == GameState::RUNTIME_EDITOR) 
            {
				EngineApp::Get().SetGameState(GameState::EDITOR);
				activeScene->OnSimulationStop();
				activeScene = editorScene;
				SceneHierarchyPanel::Get().SetContext(activeScene);
				SceneHierarchyPanel::Get().SetSelectedEntity(SceneHierarchyPanel::Get().GetNullEntity());
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}
}