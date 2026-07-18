#include "EditorLayer.h"
#include "Panels/ViewportPanel.h"
#include "Panels/DataPanel.h"
#include "Theme/IconsFontAwesome.h"
// #include "Panels/SceneHierarchyPanel.h"
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
		int mouseX = (int)mx - m_ViewportMargin.x;
		int mouseY = (int)my - m_ViewportMargin.y;

		// Convert from display-image coordinates to framebuffer coordinates.
		// The framebuffer may have a different resolution than the displayed
		// image (e.g. fixed 1280x720 fbo vs aspect-corrected m_ViewportSize).
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
		{
			auto& spec = m_Fbo->GetSpecification();
			mouseX = (int)(mouseX * ((float)spec.Width  / m_ViewportSize.x));
			mouseY = (int)(mouseY * ((float)spec.Height / m_ViewportSize.y));
		}

		DataPanel::Get().SetMouseX(mouseX);
		DataPanel::Get().SetMouseY(mouseY);

		auto& spec = m_Fbo->GetSpecification();
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)spec.Width && mouseY < (int)spec.Height)
		{
			// Record picking state; actual ReadPixel is deferred to ProcessDeferredPicking
			// at the start of the next frame (so the GPU has finished rendering the previous
			// frame's entity ID attachments before we download from them).
			m_DeferredMouseX = mouseX;
			m_DeferredMouseY = mouseY;

			if (ImGui::IsMouseClicked(0) && !ImGuizmo::IsUsing())
				m_PendingPicking = true;
		}
	}


	void ViewportPanel::ProcessDeferredPicking()
	{
		if (!m_PendingPicking || !m_Fbo || !Editor::GetActiveScene())
			return;

		m_PendingPicking = false;

		int mouseX = m_DeferredMouseX;
		int mouseY = m_DeferredMouseY;

		auto& spec = m_Fbo->GetSpecification();
		if (mouseX < 0 || mouseY < 0 || mouseX >= (int)spec.Width || mouseY >= (int)spec.Height)
			return;

		int pixelData = m_Fbo->ReadPixel(1, mouseX, mouseY);

		if (pixelData == -1 || !Editor::GetActiveScene()->GetRegistry()->valid((entt::entity)pixelData))
			Editor::SetSelectedEntity();
		else
			Editor::SetSelectedEntity(Entity((entt::entity)pixelData, Editor::GetActiveScene().get()));

		if (!Input::IsKeyDown(KeyCode::LeftAlt))
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
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

		// For EditorCamera the framebuffer is kept at the fixed reference size
		// (1280x720).  For GameCamera we use the aspect-corrected m_ViewportSize.
		// In either case only trigger a (re)build on explicit MustResize() or if
		// the fbo has never been sized yet (spec.Width == 0).
		uint32_t targetW = spec.Width, targetH = spec.Height;
		if (dynamic_cast<EditorCamera*>(camera.get()))
		{
			targetW = 1280;
			targetH = 720;
		}
		else
		{
			targetW = (uint32_t)m_ViewportSize.x;
			targetH = (uint32_t)m_ViewportSize.y;
		}

		if (m_MustResize || (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != targetW || spec.Height != targetH)))
		{
			m_MustResize = false;
			ResizeFramebufferSpecification(camera, targetW, targetH);
		}
	}

	void ViewportPanel::ResizeFramebufferSpecification(Ref<Camera> camera, uint32_t width, uint32_t height)
	{
		// EditorCamera: Keep a fixed reference resolution so resizing the
		// viewport panel never changes what the user sees (no stretch, no zoom).
		// GameCamera: Use the caller-provided size (already aspect-corrected).
		uint32_t fbWidth = width, fbHeight = height;
		if (dynamic_cast<EditorCamera*>(camera.get()))
		{
			fbWidth  = 1280;
			fbHeight = 720;
		}
		m_Fbo->Resize(fbWidth, fbHeight);
		camera->SetViewportSize(fbWidth, fbHeight);
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

		// Center GameCamera
		if (dynamic_cast<GameCamera*>(Editor::GetActiveCamera().get()))
		{
			float screenWidth  = Editor::GetActiveCamera()->GetSettings().width;
			float screenHeight = Editor::GetActiveCamera()->GetSettings().height;

			viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			
			if (viewportPanelSize.x >= viewportPanelSize.y)
				m_ViewportSize = { m_ViewportSize.y * screenWidth / screenHeight, m_ViewportSize.y };
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

			float refAspect = 1280.0f / 720.0f;
			float panelAspect = viewportPanelSize.x / viewportPanelSize.y;
			if (panelAspect >= refAspect)
			{
				m_ViewportSize.x = viewportPanelSize.y * refAspect;
				m_ViewportSize.y = viewportPanelSize.y;
			}
			else
			{
				m_ViewportSize.x = viewportPanelSize.x;
				m_ViewportSize.y = viewportPanelSize.x / refAspect;
			}
			m_ViewportMargin = { (viewportPanelSize.x - m_ViewportSize.x) * 0.5f,
								 (viewportPanelSize.y - m_ViewportSize.y) * 0.5f };
		}

		/////////////////////////
		// Resize before capturing texture ID for ImGui
		ResizeViewport(camera);

		// Center Viewport Image
		ImVec2 contentRegionSize{ (viewportPanelSize.x - m_ViewportSize.x) * 0.5f,
								  (viewportPanelSize.y - m_ViewportSize.y) * 0.5f };
		ImGui::SetCursorPos(contentRegionSize);

		uintptr_t textureID = m_Fbo->GetColorAttachmentRenderID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 0 }, ImVec2{ 1, 1 });

		/////////////////////////
		// Enable Camera Controls
		m_AllowViewportCameraEvents = (ImGui::IsMouseHoveringRect(m_MinViewportBound, m_MaxViewportBound) && m_ViewportFocused) || m_StartedCameraClickInViewport;

		if (Input::IsMouseButtonDown(MouseButton::Left) || 
			  Input::IsMouseButtonDown(MouseButton::Middle) || 
			  Input::IsMouseButtonDown(MouseButton::Right) && 
			  !m_StartedCameraClickInViewport && m_ViewportHovered)
		{
			m_StartedCameraClickInViewport = true;
		}

		if (!Input::IsMouseButtonDown(MouseButton::Right) && 
			!Input::IsKeyDown(KeyCode::LeftAlt) && 
			(Input::IsMouseButtonDown(MouseButton::Left) || (Input::IsMouseButtonDown(MouseButton::Middle))))
		{
			m_StartedCameraClickInViewport = false;
		}


		///////////////////////////////////
		// Export to DragDropViewportTarget
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) 
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				m_FilePath = PathService::ResolveAsset(path).string();

				if (Editor::SelectedEntity() && Editor::SelectedEntity().HasComponent<Render2DComponent>()) 
				{
					if (m_FilePath.extension() == ".png" || m_FilePath.extension() == ".jpg" || m_FilePath.extension() == ".jpeg")
					{
						auto textureHolder = Texture::Create(m_FilePath);
						auto& component = Editor::SelectedEntity().GetComponent<Render2DComponent>();
						component.texture = textureHolder;

						if (component.isSubTexture)
						{
							component.subTexture = SubTexture::UpdateCoords(component.texture, component.vertices,
																			component.subTextureIndex, 
																			component.subTextureCellSize,
																			component.subTextureSpriteSize);
						}
						else
						{
							component.subTexture = SubTexture::UpdateCoords(component.texture, component.vertices,
																			{0, 0}, 
																			{component.texture->GetWidth(), component.texture->GetHeight()});
						}
					}
				}
			}
			else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_PREFAB")) 
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path prefabPath = PathService::ResolveAsset(path);
				Scene::LoadPrefab(Editor::GetActiveScene().get(), prefabPath.filename().string());
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

		ImGuizmo::SetRect(m_MinViewportBound.x + contentRegionSize.x, m_MinViewportBound.y + contentRegionSize.y, 
							m_ViewportSize.x, m_ViewportSize.y);

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
	}


	void ViewportPanel::ReleaseResources()
	{
		m_AssetIconMap.clear();
	}

	void ViewportPanel::PlayButtonBar() 
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

		auto icon = EngineApp::IsEditorMode() ? ICON_FA_PLAY : ICON_FA_STOP;
		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		if (ImGui::Button(icon) || Editor::GetActiveScene()->ExitFromRuntimeEditor())
        {
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			m_GizmoType = -1;
			if (EngineApp::IsEditorMode()) 
            {
				Log::ClearLogMessages();	
				io.ConfigFlags ^= ImGuiConfigFlags_NavEnableKeyboard; 
				
				EngineApp::Get().SetSceneMode(EngineApp::SceneMode::SIMULATING);

				// Reset for physics
				EngineApp::Get().GetTimer().GetAccumulatedTime() = 0.0f;

				Editor::SetActiveScene(Scene::Copy(Editor::GetEditorScene()));
				Editor::GetActiveScene()->OnSimulationStart();
				// TODO: Fix SceneHierarchyPanel
				// SceneHierarchyPanel::Get().SetContext(Editor::GetActiveScene());
			}
			else if (EngineApp::IsSimulationMode()) 
            {
				io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
				Editor::GetActiveScene()->OnSimulationStop();

				EngineApp::Get().SetSceneMode(EngineApp::SceneMode::EDITOR);

				Editor::SetActiveScene(Editor::GetEditorScene());
				// TODO: Fix SceneHierarchyPanel
				// SceneHierarchyPanel::Get().SetContext(Editor::GetActiveScene());

				// Provisional fix to avoid crash
				NativeScriptFn::FreeScriptLibrary();
				Editor::SetSelectedEntity();
			}
		}

		// If inside the game we want to quit, the state of the editor will exit from Simulation
		if (EngineApp::IsSceneExit()) 
		{
			Editor::GetActiveScene()->OnSimulationStop();

			EngineApp::Get().SetSceneMode(EngineApp::SceneMode::EDITOR);

			Editor::SetActiveScene(Editor::GetEditorScene());
			// TODO: Fix SceneHierarchyPanel
			// SceneHierarchyPanel::Get().SetContext(Editor::GetActiveScene());

			// Provisional fix to avoid crash
			NativeScriptFn::FreeScriptLibrary();
			Editor::SetSelectedEntity();
		}
		else if (EngineApp::IsSimulationMode())
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