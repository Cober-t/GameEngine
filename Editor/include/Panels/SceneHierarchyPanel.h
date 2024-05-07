#ifndef SCENE_HIERARCHY_PANEL_H
#define SCENE_HIERARCHY_PANEL_H

#include "Scene/ECS.h"

#include "Theme/IconsFontAwesome.h"
#include <filesystem>
#include <ImGui/imgui_internal.h>

#include <string_view>

namespace Cober {

	namespace ComponentNames {

		constexpr std::string_view Transform = "Transform";
		constexpr std::string_view Rigidbody2D = "Rigidbody2D";
		constexpr std::string_view Box2DCollider = "Box 2D Collider";
		constexpr std::string_view Circle2DCollider = "Circle 2D Collider";
		constexpr std::string_view Render2DShape = "Render 2D Shape";
		constexpr std::string_view NativeScript = "Native Script";
	}

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
		std::map<std::string, Ref<Texture>> m_AssetIconMap;

	private:
		static SceneHierarchyPanel* s_Instance;
	};
}

#endif