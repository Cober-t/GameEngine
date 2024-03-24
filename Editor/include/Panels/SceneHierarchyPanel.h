#ifndef SCENE_HIERARCHY_PANEL_H
#define SCENE_HIERARCHY_PANEL_H

#include "Scene/ECS.h"

#include <filesystem>

namespace Cober {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		~SceneHierarchyPanel();

		static SceneHierarchyPanel& Get() { return *s_Instance; }

		void OnGuiRender(Entity& hoveredEntity);

		void SetContext(const Ref<Scene>& selectionContext);

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		Entity GetNullEntity() const { return m_NullEntityContext; }
		void SetSelectedEntity(Entity entity);
		void SetNullEntityContext();

		template<typename T, typename UIFunction>
		void DrawComponent(const std::string& name, Entity& entity, UIFunction uiFunction);

		template<typename T>
		void AddIfHasComponent(std::string name);
	private:
		void DrawEntityNode(Entity entity, Entity& hoveredEntity);
		void DrawComponents(Entity& entity);
	private:
		Ref<Scene> m_SceneContext;
		Entity m_SelectionContext;
		Entity m_NullEntityContext;
		std::string m_NewEntityGroup;
		std::string m_NewEntityTag;
	private:
		static SceneHierarchyPanel* s_Instance;
	};
}

#endif