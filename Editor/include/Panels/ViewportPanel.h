#ifndef VIEWPORT_PANEL_H
#define VIEWPORT_PANEL_H

#include "Theme/EditorResources.h"

#include <Engine.h>
#include <imgui/imgui.h>

namespace Cober {

	class ViewportPanel 
	{
	public:
		ViewportPanel();
		~ViewportPanel();

		static ViewportPanel& Get() { return *m_Instance; }
		
		// void RenderSkybox();
		void CreateFramebuffer(uint32_t width = 1280, uint32_t height = 720);
		Ref<Framebuffer>& GetFramebuffer() { return m_Fbo; };

		void BindFramebuffer();
		void UnbindFramebuffer();
		void FBOClearAttachments(uint32_t attachmentIndex, int value) { m_Fbo->ClearAttachment(attachmentIndex, value); }

		void OnEvent(Event& event);	// Abstract to EVENT API
		void ResizeViewport(Ref<Camera> editorCamera);
		void ResizeFramebufferSpecification(Ref<Camera> editorCamera, uint32_t width, uint32_t height);

		void OnGuiRender(Ref<EditorCamera>& editorCamera, Ref<Camera>& camera);

		void SetCursorEntity();
		void MustResize(bool resize = true) { m_MustResize = resize; }; 
		void PlayButtonBar(EngineApp::GameState gameState);
		inline bool AllowViewportCameraEvents() { return m_AllowViewportCameraEvents; }

	private:
		Ref<Framebuffer> m_Fbo;
		std::map<std::string, Ref<Texture>> m_AssetIconMap;
		static ViewportPanel* m_Instance;

		glm::vec2 m_ViewportMargin = { 0.0f, 0.0f };
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		ImVec2 m_MinViewportBound, m_MaxViewportBound;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		bool m_StartedCameraClickInViewport = false, m_AllowViewportCameraEvents = false;
		bool m_MustResize = false;

		bool m_MouseButtonHeld = false;
		glm::vec2 m_Mouse{0.0f, 0.0f}, m_LastMousePos{ 0.0f, 0.0f };

		std::filesystem::path m_FilePath;
		int m_GizmoType = -1;
	};
}

#endif