#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include <Engine.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// GUI PANELS
/*
#include "Panels/MenuPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/MenuPanel.h"
#include "Panels/DataPanel.h"
#include "Panels/ViewportPanel.h"
*/

namespace Cober {

	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		//void OnEvent(SDL_Event& event) override;

	private:
		void InitDockspace();
		void EndDockspace();
		//bool OnKeyPressed(KeyPressedEvent& event)
	private:
		//Ref<EditorCamera> _editorCamera;
		//Ref<Scene> _activeScene, _editorScene;
	private:
		//Ref<Framebuffer> _fbo;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_MinViewportBound;
		glm::vec2 m_MaxViewportBound;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		int m_GuizmoType = -1;
	};
}

#endif