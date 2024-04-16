#ifndef VIEWPORT_PANEL_H
#define VIEWPORT_PANEL_H

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

		void BindFramebuffer();
		void UnbindFramebuffer();
		void FBOClearAttachments(uint32_t attachmentIndex, int value) { m_Fbo->ClearAttachment(attachmentIndex, value); }

		void OnEvent(Event& event, Entity& hoveredEntity);	// Abstract to EVENT API
		void ResizeViewport(Ref<EditorCamera> editorCamera, bool& game2D);
		void ResizeFramebufferSpecification(Ref<EditorCamera> editorCamera, uint32_t width, uint32_t height);

		void OnGuiRender(Ref<EditorCamera> editorCamera, Ref<Scene>& scene);

		void SetCursorEntity(Ref<Scene>& activeScene, Entity& hoveredEntity);
		void PlayButtonBar(Ref<Scene>& editorScene, Ref<Scene>& activeScene, GameState gameState);
		inline bool AllowViewportCameraEvents() { return m_AllowViewportCameraEvents; }

	private:
		Ref<Framebuffer> m_Fbo;
		Ref<EditorCamera> m_CameraAux;
		static ViewportPanel* m_Instance;

		glm::vec2 m_ViewportMargin = { 0.0f, 0.0f };
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		ImVec2 m_MinViewportBound, m_MaxViewportBound;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		bool m_StartedCameraClickInViewport = false, m_AllowViewportCameraEvents = false;

		bool m_MouseButtonHeld = false;
		glm::vec2 m_Mouse{0.0f, 0.0f}, m_LastMousePos{ 0.0f, 0.0f };

		std::string m_FilePath;
		int m_GizmoType = -1;
	};
}

#endif