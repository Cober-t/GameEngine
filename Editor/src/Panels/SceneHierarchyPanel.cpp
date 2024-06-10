#include "Theme/EditorResources.h"

#include "EditorLayer.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Render/Text/Font.h"
//#include "Cober/Renderer/Renderer.h"
//#include "Cober/Renderer/Lighting.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Cober {

	bool m_LoadScripts = true;
	const char* m_CurrentScript = "None";
	std::vector<std::string> m_ScriptList;
	SceneHierarchyPanel* SceneHierarchyPanel::s_Instance = nullptr;
	
	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		s_Instance = this;

		m_AssetIconMap["audio"] = EditorResources::AudioIcon;
		m_AssetIconMap["boxCollider2D"] = EditorResources::BoxCollider2DIcon;
		m_AssetIconMap["circleCollider2D"] = EditorResources::CircleCollider2DIcon;
		m_AssetIconMap["rigidbody2D"] = EditorResources::RigidBody2DIcon;
		m_AssetIconMap["nativeScript"] = EditorResources::NativeScriptIcon;
		m_AssetIconMap["sprite"] = EditorResources::SpriteIcon;
		m_AssetIconMap["transform"] = EditorResources::TransformIcon;
		m_AssetIconMap["remove"] = EditorResources::ClearIcon;
	}


	SceneHierarchyPanel::~SceneHierarchyPanel() 
	{
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


	void SceneHierarchyPanel::OnGuiRender() 
    {

		ImGui::Begin("Scene Hierarchy");

		for (auto& entity : m_SceneContext->GetSceneEntities()) 
		{
			DrawEntityNode(entity);
			if (ImGui::BeginDragDropSource())
			{
				uint64_t id = (uint64_t)entity.GetUUID();
				ImGui::SetDragDropPayload("ENTITY_TYPE", &id, sizeof(uint64_t));
				ImGui::EndDragDropSource();
			}
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) 
		{
			m_SelectionContext = m_NullEntityContext;
			Editor::SetSelectedEntity(m_SelectionContext);
		}

		if (ImGui::BeginPopupContextWindow(0, 1)) 
		{
			if (m_SelectionContext == m_NullEntityContext && ImGui::Selectable("Empty Entity")) 
			{
				m_SelectionContext = m_SceneContext->CreateEntity();
				Editor::SetSelectedEntity(m_SelectionContext);
			}
				
			ImGui::EndPopup();
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext != m_NullEntityContext)
			DrawComponents(m_SelectionContext);
			
		ImGui::End();
	}


	void SceneHierarchyPanel::DrawEntityNode(Entity entity) 
    {

		auto& tag = entity.GetComponent<TagComponent>().tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		// flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked()) 
        {
			m_SelectionContext = entity;
			Editor::SetSelectedEntity(m_SelectionContext);
		}

		// Delete an Entity
		bool entityDeleted = false;
		if (m_SelectionContext == entity && ImGui::BeginPopupContextWindow(0, 1)) 
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();

		if (entityDeleted) 
		{
			if (m_SelectionContext == entity) 
			{
				m_SelectionContext = m_NullEntityContext;
				Editor::SetSelectedEntity();
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

		if (entity.HasComponent<T>()) 
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y;

			// Switch to make icon components
			Ref<Texture> iconComponent = m_AssetIconMap["transform"];

			if (name == ComponentNames::Transform)
				iconComponent = m_AssetIconMap["transform"];
			else if (name == ComponentNames::Rigidbody2D)
				iconComponent = m_AssetIconMap["rigidbody2D"];
			else if (name == ComponentNames::Circle2DCollider)
				iconComponent = m_AssetIconMap["circleCollider2D"];
			else if (name == ComponentNames::Box2DCollider)
				iconComponent = m_AssetIconMap["boxCollider2D"];
			else if (name == ComponentNames::Render2DShape)
				iconComponent = m_AssetIconMap["sprite"];


			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			ImGui::Separator();
			// Add icons to a tree with monospace FontAwesome icons (where iconComponent is a 'const char*')
			// bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s %s", iconComponent, name.c_str());
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight - 5.0f, 0.0f);

			if (name != ComponentNames::Transform)
			{
				if (ImGui::ImageButton((ImTextureID)m_AssetIconMap["remove"]->GetRendererID(), ImVec2{ lineHeight*0.9f, lineHeight*0.9f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
					entity.RemoveComponent<T>();
			}
				
			ImGui::SameLine(2.0f, 0.0f);
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			// This usless button cover the node tree's ugly arrow
			ImGui::ImageButton((ImTextureID)iconComponent->GetRendererID(), ImVec2{ lineHeight*0.9f, lineHeight*0.9f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	        ImGui::PopItemFlag();

			if (open) 
			{
				uiFunction(component, entity);
				ImGui::TreePop();
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
			entity.GetComponent<TagComponent>().tag = (std::string)buffer;

		if (ImGui::Button("Add Component")) 
        {
			if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) 
            {
				if (!m_SelectionContext.HasComponent<CameraComponent>()) 
                {
					m_SelectionContext.AddComponent<CameraComponent>();
				}
				if (!m_SelectionContext.HasComponent<Rigidbody2D>()) 
                {
					m_SelectionContext.AddComponent<Rigidbody2D>();
				}
				else if (!m_SelectionContext.HasComponent<BoxCollider2D>()) 
                {
					m_SelectionContext.AddComponent<BoxCollider2D>();
				}
				else if (!m_SelectionContext.HasComponent<CircleCollider2D>()) 
                {
					m_SelectionContext.AddComponent<CircleCollider2D>();
				}
				// else if (!m_SelectionContext.HasComponent<EdgeCollider2D>()) 
                // {
				// 	m_SelectionContext.AddComponent<EdgeCollider2D>();
				// }
				// else if (!m_SelectionContext.HasComponent<PolygonCollider2D>()) 
                // {
				// 	m_SelectionContext.AddComponent<PolygonCollider2D>();
				// }
				else if (!m_SelectionContext.HasComponent<Render2DComponent>()) 
                {
					m_SelectionContext.AddComponent<Render2DComponent>();
				}
				else if (!m_SelectionContext.HasComponent<NativeScriptComponent>())
				{
					m_SelectionContext.AddComponent<NativeScriptComponent>();
				}
				else if (!m_SelectionContext.HasComponent<AudioComponent>())
				{
					m_SelectionContext.AddComponent<AudioComponent>();
				}
				else if (!m_SelectionContext.HasComponent<TextComponent>())
				{
					m_SelectionContext.AddComponent<TextComponent>();
				}
			}
			else
				ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent")) 
		{
			AddIfHasComponent<CameraComponent>((std::string)ComponentNames::Camera);
			AddIfHasComponent<Rigidbody2D>((std::string)ComponentNames::Rigidbody2D);
			AddIfHasComponent<BoxCollider2D>((std::string)ComponentNames::Box2DCollider);
			AddIfHasComponent<CircleCollider2D>((std::string)ComponentNames::Circle2DCollider);
			// AddIfHasComponent<EdgeCollider2D>("Edge Collider 2D Component");
			// AddIfHasComponent<PolygonCollider2D>("Polygon Collider 2D Component");
			AddIfHasComponent<Render2DComponent>((std::string)ComponentNames::Render2DShape);
			AddIfHasComponent<NativeScriptComponent>((std::string)ComponentNames::NativeScript);
			AddIfHasComponent<AudioComponent>((std::string)ComponentNames::Audio);
			AddIfHasComponent<TextComponent>((std::string)ComponentNames::Text);
			// ...
			// ...

			ImGui::EndPopup();
		}

		DrawComponent<TransformComponent>((std::string)ComponentNames::Transform, entity, [](auto& component, auto& entity)
			{
				DrawVec3Control("Position", component.position);
				glm::vec3 rotation = glm::degrees(component.rotation);
				DrawVec3Control("Rotation", rotation);
				component.rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.scale, 1.0f);
			});

		DrawComponent<CameraComponent>((std::string)ComponentNames::Camera, entity, [](auto& component, auto& entity)
			{
				// DrawVec3Control("Focal Point", component.focalPoint);
				if (ImGui::DragFloat("Distance", &component.distance, 0.1, 0.0f, 100.0f))
				{
					component.camera->GetSettings().distance = component.distance;
					component.camera->SetViewportSize(component.width, component.height);
					ViewportPanel::Get().MustResize();
				}

				if (ImGui::DragInt("Width", &component.width, 1, 1))
				{
					component.camera->GetSettings().width = component.width;
					component.camera->SetViewportSize(component.width, component.height);
				}

				if (ImGui::DragInt("Height", &component.height, 1, 1))
				{
					component.camera->GetSettings().height = component.height;
					component.camera->SetViewportSize(component.width, component.height);
				}

				if (ImGui::DragFloat("Near Clip", &component.nearClip, 10.0f, 0.0f, 1000.0f))
				{
					component.camera->GetSettings().nearClip = component.nearClip;
					component.camera->SetViewportSize(component.width, component.height);
					ViewportPanel::Get().MustResize();
				}

				if (ImGui::DragFloat("Far Clip", &component.farClip, 10.0f, 0.0f, 1000.0f))
				{
					component.camera->GetSettings().nearClip = component.nearClip;
					component.camera->SetViewportSize(component.width, component.height);
					ViewportPanel::Get().MustResize();
				}

				if (ImGui::DragFloat("Fov", &component.fov, 0.01f, 0.0f, 90.0f))
				{
					component.camera->GetSettings().fov = component.fov;
					component.camera->SetViewportSize(component.width, component.height);
					ViewportPanel::Get().MustResize();
				}

				if (ImGui::Checkbox("Perspective", &component.perspective))
				{
					component.camera->GetSettings().perspectiveProjection = component.perspective;
				    ViewportPanel::Get().MustResize();
				}
					
				if (ImGui::Checkbox("Main Camera", &component.mainCamera))
				{
					component.camera->SetMainCamera(component.mainCamera);

					if (component.mainCamera)
						Editor::SetMainCamera(component.camera);
					else
						Editor::SetMainCamera(Editor::GetEditorCamera());
						
					ViewportPanel::Get().MustResize();
				}
				ImGui::Checkbox("Debug", &component.debug);
			});

		DrawComponent<Rigidbody2D>((std::string)ComponentNames::Rigidbody2D, entity, [](auto& component, auto& entity)
			{
				const char* bodyTypeStrings[] = { "Static", "Kinematic", "Dynamic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.type];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString)) {

					for (int i = 0; i < 3; i++) {
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected)) 
						{
							currentBodyTypeString = bodyTypeStrings[i];
							Physics2D::SetBodyType(entity, (BodyType)i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				ImGui::Checkbox("Fixed Rotation", &component.fixedRotation);
			});

		DrawComponent<BoxCollider2D>((std::string)ComponentNames::Box2DCollider, entity, [](auto& component, auto& entity)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
				ImGui::DragFloat2("Size", glm::value_ptr(component.size), 1.0f, 1.0f);
				ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
				ImGui::Checkbox("Is Sensor", &component.isSensor);
			});


		DrawComponent<CircleCollider2D>((std::string)ComponentNames::Circle2DCollider, entity, [](auto& component, auto& entity)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
				ImGui::DragFloat("Radius", &component.radius, 0.5f, 0.0f, 100.0f);
				ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
				ImGui::Checkbox("Is Sensor", &component.isSensor);
			});


		DrawComponent<EdgeCollider2D>("Edge Collider 2D", entity, [](auto& component, auto& entity)
			{
				ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
			});


		DrawComponent<PolygonCollider2D>("Polygon Collider 2D", entity, [](auto& component, auto& entity)
			{
				ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
			});
		

		DrawComponent<Render2DComponent>((std::string)ComponentNames::Render2DShape, entity, [](auto& component, auto& entity)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.color));

				const char* shape2DString[] = { "Line", "Quad", "Circle", "Sprite"};
				const char* current2DShape = shape2DString[(int)component.shapeType];
				if (ImGui::BeginCombo("2D Shape", current2DShape)) {

					for (int i = 0; i < (int)Shape2D::N_SHAPE_ITEMS; i++) {
						bool isSelected = current2DShape == shape2DString[i];
						if (ImGui::Selectable(shape2DString[i], isSelected)) 
						{
							current2DShape = shape2DString[i];
							component.shapeType = (Shape2D)i;
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
			
				if ((int)component.shapeType == (int)Shape2D::Line)
				{
					DrawVec3Control("Point 1", component.point1);
					DrawVec3Control("Point 2", component.point2);
					ImGui::DragFloat("Thickness", &component.thickness, 0.1f, 0.1f, 5.0f);
				}

				if ((int)component.shapeType == (int)Shape2D::Quad)
				{
					ImGui::Checkbox("Fill", &component.fill);
				}

				if ((int)component.shapeType == (int)Shape2D::Circle)
				{
					ImGui::DragFloat("Thickness", &component.thickness, 0.1f, 0.1f, 1.0f);
					ImGui::DragFloat("Fade", &component.fade, 0.0f, 0.0f, 1.0f);
				}

				if ((int)component.shapeType == (int)Shape2D::Sprite)
				{
					std::string nameTexture = component.texture == nullptr ? "Texture" : component.texture->GetName();
					ImGui::Button(nameTexture.c_str(), ImVec2(100.0f, 0.0f));

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path texturePath = std::filesystem::current_path() / "assets" / path;

							std::string format = texturePath.string();
							auto lastDot = format.find_last_of('.');
							format = lastDot != std::string::npos ? format.substr(lastDot) : "null";

							if (lastDot != std::string::npos && (format == ".png" || format == ".jpg" || format == ".jpeg"))
								component.texture = Texture::Create(texturePath.string());
						}
						ImGui::EndDragDropTarget();
					}
				}
			});

		DrawComponent<NativeScriptComponent>((std::string)ComponentNames::NativeScript, entity, [](auto& component, auto& entity)
			{
				if (m_LoadScripts)
				{
					// File watcher in the future
					m_ScriptList.erase(m_ScriptList.begin(), m_ScriptList.end());
					std::filesystem::path scriptPath = std::filesystem::current_path() / "ScriptModule" / "scripts";
					for (const auto & entry : std::filesystem::directory_iterator(scriptPath))
					{
						std::string stringPath = entry.path().string();
						if (!std::filesystem::is_directory(entry.path()) && 
							stringPath.substr(stringPath.find_last_of(".")) == ".h")
						{
							std::string fileName = entry.path().filename().string();
							fileName = fileName.substr(0, fileName.find_last_of("."));
							m_ScriptList.push_back(fileName);
						}
					}

					m_LoadScripts = false;
				}

				if (ImGui::Button(ICON_FA_REFRESH)) 
					m_LoadScripts = true;

				ImGui::SameLine();

				if (ImGui::BeginCombo("NativeScript", component.className.c_str())) 
				{
					for (int i = 0; i < m_ScriptList.size(); i++) 
					{
						bool isSelected = m_CurrentScript == m_ScriptList[i].c_str();
						if (ImGui::Selectable(m_ScriptList[i].c_str(), isSelected)) 
						{
							m_CurrentScript = m_ScriptList[i].c_str();
							component.className = m_CurrentScript;
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
			});
		
		DrawComponent<AudioComponent>((std::string)ComponentNames::Audio, entity, [](auto& component, auto& entity)
			{
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), component.audioName.c_str());
				std::filesystem::path audioPath = std::filesystem::current_path() / "assets";

				if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
				{
					component.audioPath = audioPath / "audio" / (std::string)buffer;

					std::string format = (std::string)buffer;
					auto lastDot = format.find_last_of('.');
					format = lastDot != std::string::npos ? format.substr(lastDot) : "null";
					component.audioName = (std::string)buffer;
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
						const wchar_t* path = (const wchar_t*)payload->Data;

						component.audioPath = audioPath / path;

						std::string format = component.audioPath.string();
						auto lastDot = format.find_last_of('.');
						format = lastDot != std::string::npos ? format.substr(lastDot) : "null";

						if (format == ".mp3" || format == ".wav" && std::filesystem::exists(audioPath / path))
						{
							component.audioName = component.audioPath.filename().string();
							strcpy_s(buffer, sizeof(buffer), component.audioName.c_str());
						}
					}
					ImGui::EndDragDropTarget();
				}

				if (ImGui::Checkbox("Loop", &component.loop))
				{
					Audio::LoopSound(component.audioName, component.loop);
				}
			});
		

		DrawComponent<TextComponent>((std::string)ComponentNames::Text, entity, [](auto& component, auto& entity)
			{
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), component.Text.c_str());

				if (ImGui::InputTextMultiline("##", buffer, sizeof(buffer)))
				{
					component.Text = (std::string)buffer;
				}
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Kerning", &component.Kerning, 0.025f);
				ImGui::DragFloat("Line Spacing", &component.LineSpacing, 0.025f);
			});
	}
}