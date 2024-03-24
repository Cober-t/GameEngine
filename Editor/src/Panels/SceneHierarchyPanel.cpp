#include "EditorLayer.h"
#include "Panels/SceneHierarchyPanel.h"
//#include "Cober/Scene/Components.h"
//#include "Cober/Renderer/Renderer.h"
//#include "Cober/Renderer/Lighting.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Cober {

	SceneHierarchyPanel* SceneHierarchyPanel::s_Instance = nullptr;
	
	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		s_Instance = this;
	}


	SceneHierarchyPanel::~SceneHierarchyPanel() {

		delete s_Instance;
		s_Instance = nullptr;
	}


	void SceneHierarchyPanel::SetContext(const Ref<Scene>& sceneContext)
	{
		m_SceneContext = sceneContext;
		m_NullEntityContext = Entity();
		m_SelectionContext = m_NullEntityContext;
	}


	void SceneHierarchyPanel::SetSelectedEntity(Entity entity) 
    {
		m_SelectionContext = entity;
	}


	void SceneHierarchyPanel::SetNullEntityContext() 
    {
		m_SelectionContext = m_NullEntityContext;
	}


	void SceneHierarchyPanel::OnGuiRender(Entity& hoveredEntity) 
    {

		ImGui::Begin("Scene Hierarchy");

		for (auto& entity : m_SceneContext->GetSceneEntities()) 
		{
			DrawEntityNode(entity, hoveredEntity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) 
		{
			m_SelectionContext = m_NullEntityContext;
			hoveredEntity = m_SelectionContext;
		}

		if (ImGui::BeginPopupContextWindow(0, 1)) 
		{
			if (m_SelectionContext == m_NullEntityContext && ImGui::Selectable("Empty Entity")) 
			{
				m_SelectionContext = m_SceneContext->CreateEntity();
				hoveredEntity = m_SelectionContext;
			}
				
			ImGui::EndPopup();
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext != m_NullEntityContext)
			DrawComponents(m_SelectionContext);

		ImGui::End();
	}


	void SceneHierarchyPanel::DrawEntityNode(Entity entity, Entity& hoveredEntity) 
    {

		auto& tag = entity.GetComponent<TagComponent>().tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked()) 
        {
			m_SelectionContext = entity;
			hoveredEntity = m_SelectionContext;
		}

		// Delete an Entity
		bool entityDeleted = false;
		if (m_SelectionContext == entity && ImGui::BeginPopupContextWindow(0, 1)) {
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();

		if (entityDeleted) {
			if (m_SelectionContext == entity) {
				m_SelectionContext = m_NullEntityContext;
				hoveredEntity = m_NullEntityContext;
			}
			m_SceneContext->DestroyEntity(entity);
		}
	}


	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}


	template<typename T, typename UIFunction>
	void SceneHierarchyPanel::DrawComponent(const std::string& name, Entity& entity, UIFunction uiFunction) 
    {

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>()) {

			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open) {
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent) 
			{
				entity.RemoveComponent<T>();
			}
		}
	}


	template<typename T>
	void SceneHierarchyPanel::AddIfHasComponent(std::string name) 
    {
		if (!m_SelectionContext.HasComponent<T>()) 
		{
			if (ImGui::MenuItem(name.c_str())) 
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}


	void SceneHierarchyPanel::DrawComponents(Entity& entity)
	{
		// TAGS
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), entity.GetComponent<TagComponent>().tag.c_str());

		if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			m_NewEntityTag = buffer;

		ImGui::SameLine();

		if (ImGui::Button("Rename")) 
        {
			if (m_NewEntityTag != "") 
				entity.GetComponent<TagComponent>().tag = (std::string)m_NewEntityTag;
		}

		ImGui::PushItemWidth(100.0f);

		// GROUPS
		// if (ImGui::BeginCombo("Group", entity.GetGroup().c_str())) 
        // {
		// 	auto groupList = entity.registry->GetAllGroups();
		// 	for (int n = 0; n < groupList.size(); n++) {
		// 		bool selected = (entity.GetGroup() == groupList[n]);
		// 		if (ImGui::Selectable(groupList[n].c_str(), selected))
		// 			entity.SetGroup(groupList[n]);
		// 	}
		// 	ImGui::EndCombo();
		// }

		ImGui::SameLine();
		ImGui::PushItemWidth(80.0f);

		memset(buffer, 0, sizeof(buffer));
		//strcpy_s(buffer, sizeof(buffer), entity.GetGroup().c_str());
		if (ImGui::InputText("NewGroup", buffer, sizeof(buffer)))
			m_NewEntityGroup = buffer;

		ImGui::SameLine();
		// if (ImGui::Button("+")) {
		// 	if (m_NewEntityGroup != "")
		// 		entity.SetGroup(m_NewEntityGroup);
		// }

		ImGui::PopItemWidth();
		ImGui::PopItemWidth();

		if (ImGui::Button("Add Component")) 
        {
			if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) 
            {
				if (!m_SelectionContext.HasComponent<Rigidbody2D>()) 
                {
					m_SelectionContext.AddComponent<Rigidbody2D>();
				}
				else if (!m_SelectionContext.HasComponent<BoxCollider2D>()) 
                {
					m_SelectionContext.AddComponent<BoxCollider2D>();
				}
				//else if (!m_SelectionContext.HasComponent<Script>()) {
				//	m_SelectionContext.AddComponent<Script>();
				//}
			}
			else
				ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent")) {
			AddIfHasComponent<Rigidbody2D>("Rigidbody 2D Component");
			AddIfHasComponent<BoxCollider2D>("BoxCollider 2D Component");
			//AddIfHasComponent<Script>("Script Component");
			// ...
			// ...

			ImGui::EndPopup();
		}

		DrawComponent<TransformComponent>("TransformComponent", entity, [](auto& component)
			{
				DrawVec3Control("Position", component.position);
				glm::vec3 rotation = glm::degrees(component.rotation);
				DrawVec3Control("Rotation", rotation);
				component.rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.scale, 1.0f);
			});
	

		DrawComponent<Rigidbody2D>("Rigidbody 2D", entity, [](auto& component)
			{
				const char* bodyTypeStrings[] = { "Static", "Kinematic", "Dynamic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.type];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString)) {

					for (int i = 0; i < 3; i++) {
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected)) {
							currentBodyTypeString = bodyTypeStrings[i];
							component.type = (BodyType)i;
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				ImGui::Checkbox("Fixed Rotation", &component.fixedRotation);
			});

		DrawComponent<BoxCollider2D>("Box Collider 2D", entity, [](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
				ImGui::DragFloat2("Size", glm::value_ptr(component.size), 1.0f, 1.0f);
				ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
				//ImGui::DragFloat("Restitution Threshold", &component.restitutionThreshold, 0.01f, 0.0f);
			});

		/*
		DrawComponent<Sprite>("Sprite", entity, [](auto& component)
			{
				if (ImGui::BeginDragDropSource())
				{
					if (ImGui::BeginCombo("", currentScreenSize)) {
						for (int n = 0; n < IM_ARRAYSIZE(_screenValues); n++) {
							bool selected = (currentScreenSize == _screenValues[n]);
							if (ImGui::Selectable(_screenValues[n], selected)) {
								currentScreenSize = _screenValues[n];
								switch (n) {
								case SCREEN_SIZE::VERY_LOW:	 Resize(editorCamera, activeScene, _VPSize[n].x, _VPSize[n].y, game2D);	break;
								case SCREEN_SIZE::LOW:		 Resize(editorCamera, activeScene, _VPSize[n].x, _VPSize[n].y, game2D);	break;
								case SCREEN_SIZE::MID:		 Resize(editorCamera, activeScene, _VPSize[n].x, _VPSize[n].y, game2D);	break;
								case SCREEN_SIZE::HIGH:		 Resize(editorCamera, activeScene, _VPSize[n].x, _VPSize[n].y, game2D);	break;
								case SCREEN_SIZE::VERY_HIGH: Resize(editorCamera, activeScene, _VPSize[n].x, _VPSize[n].y, game2D);	break;
								case SCREEN_SIZE::ULTRA:	 Resize(editorCamera, activeScene, _VPSize[n].x, _VPSize[n].y, game2D);	break;
								}
							}
						}
						ImGui::EndCombo();
					}
					ImGui::EndDragDropSource();
				}

			});
		*/
		// Draw Rigidbody3D Component

		// Draw BoxCollider3D Component
	}
}