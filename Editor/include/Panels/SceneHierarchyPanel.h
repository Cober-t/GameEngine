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

		static SceneHierarchyPanel& Get() { return *instance; }

		void OnGuiRender(Entity& hoveredEntity);

		void SetContext(const Ref<Scene>& selectionContext);

		Entity GetSelectedEntity() const { return _selectionContext; }
		Entity GetNullEntity() const { return _nullEntityContext; }
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
		Ref<Scene> _sceneContext;
		Entity _selectionContext;
		Entity _nullEntityContext;
		std::string _newEntityGroup;
		std::string _newEntityTag;
	private:
		static SceneHierarchyPanel* instance;
	};
}

#endif