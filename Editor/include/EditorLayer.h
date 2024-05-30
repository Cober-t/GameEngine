#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include <Engine.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// GUI PANELS
#include "Panels/ViewportPanel.h"
#include "Panels/DataPanel.h"
#include "Panels/ConsolePanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/MenuPanel.h"

#include "Physics/Debug2DPhysics.h"

namespace Cober {

	class Editor : public Layer 
	{
	public:
		Editor();
		virtual ~Editor() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Unique<Timestep>& ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		static Ref<Scene>& GetActiveScene();
		static Ref<Scene>& GetEditorScene();
		static Ref<EditorCamera>& GetEditorCamera();
		static Entity& SelectedEntity();
		static void SetActiveScene(Ref<Scene>& scene);
		static void SetEditorScene(Ref<Scene>& scene);
		static void SetSelectedEntity(Entity& entity = Entity());

		static Ref<Camera>& GetActiveCamera();
		static void SetMainCamera(Ref<Camera>& entity);
		static void SetMainCamera(Ref<EditorCamera>& entity);

	private:
		void InitDockspace();
		void EndDockspace();
		bool OnKeyPressed(KeyPressedEvent& event);

	private:
		Ref<Framebuffer> m_Fbo;
		bool m_AllowViewportCameraEvents;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_MinViewportBound;
		glm::vec2 m_MaxViewportBound;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		int m_GuizmoType = -1;
	};
}

#endif