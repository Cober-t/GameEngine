#include <pch.h>

#include "EditorLayer.h"
#include "Panels/ContentBrowserPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Cober {

	ContentBrowserPanel* ContentBrowserPanel::s_Instance = nullptr;

	ContentBrowserPanel::ContentBrowserPanel()
	{
		s_Instance = this;
		m_AssetsPath = PathService::AssetsRoot();
		m_CurrentDirectory = PathService::AssetsRoot();

		m_AssetIconMap[".png"] = EditorResources::PNGFileIcon;
		m_AssetIconMap[".jpg"] = EditorResources::JPGFileIcon;

		m_AssetIconMap["folder"] = EditorResources::FolderIcon;
		m_AssetIconMap["file"] = EditorResources::FileIcon;
		m_AssetIconMap["backwards"] = EditorResources::BackwardsIcon;
	}


	ContentBrowserPanel::~ContentBrowserPanel() 
	{
		delete s_Instance;
		s_Instance = nullptr;
	}

	void ContentBrowserPanel::ReleaseResources()
	{
		m_AssetIconMap.clear();
		m_TextureFolderContentHolder.clear();
	}


	void ContentBrowserPanel::OnGuiRender()
	{
		ImGui::Begin("Content Browser");

		ImRect inner_rect = ImGui::GetCurrentWindow()->InnerRect;
		if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("##WindowBgArea")))
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_TYPE"))
			{
				uint64_t uuid;
				memcpy((uint64_t*)&uuid, payload->Data, sizeof(uint64_t));
				auto entity = Editor::GetActiveScene()->GetEntityByUUID(uuid);
				EntitySerializer::Serialize(entity, entity.GetName() + ".lua");
			}
			ImGui::EndDragDropTarget();
		}

		if (m_CurrentDirectory != m_AssetsPath)
		{
			const char* strID = "backwards";
			if (ImGui::ImageButton(strID, (void*)m_AssetIconMap["backwards"]->GetRendererID(), ImVec2(18.0f, 18.0f), { 0, 0 }, { 1, 1 }))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path().string() + "\\";
				m_TextureFolderContentHolder.clear();
			}
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Export global colors
			ImGui::Image((ImTextureID)m_AssetIconMap["backwards"]->GetRendererID(), ImVec2(18.0f, 18.0f), { 0, 0 }, { 1, 1 });
			ImGui::PopStyleColor();
		}

		static float padding = 10.0f;
		static float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);

		if (columnCount < 1)
			columnCount = 1;

		ImGui::PushItemWidth(100.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 85, "%.0f"); // Export to Settings
		ImGui::SameLine();
		ImGui::SliderFloat("Padding", &padding, 0, 32, "%.0f"); // Export to Settings
		ImGui::PopItemWidth();
		

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 10 });

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) 
        {
			const auto& path = directoryEntry.path();
			std::filesystem::path relativePath = std::filesystem::relative(path, m_AssetsPath);
			std::string filenameString = relativePath.filename().string();

			ImGui::PushID(filenameString.c_str());
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Export global colors

			if (directoryEntry.is_directory())
			{
				const char* strID = "folder";
				ImGui::ImageButton(strID, (void*)m_AssetIconMap["folder"]->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 0 }, { 1, 1 });
			}
			else
			{
				// Thumnails (Make an enum for file types)
				if (relativePath.extension() == ".png" || relativePath.extension() == ".jpg")
				{
					std::filesystem::path texturePath = m_CurrentDirectory / filenameString;
					if (m_TextureFolderContentHolder.find(texturePath) == m_TextureFolderContentHolder.end())
					{
						m_TextureFolderContentHolder[texturePath] = Texture::Create(texturePath);
					}
					
					const char* strID = "imageFile";
					ImGui::ImageButton(strID, (void*)m_TextureFolderContentHolder[texturePath]->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 0 }, { 1, 1 });
				}
				else {

					const char* strID = "file";
					ImGui::ImageButton(strID, (void*)m_AssetIconMap["file"]->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 0 }, { 1, 1 });
				}
			}

			// Drag and Drop
			if (ImGui::BeginDragDropSource()) 
            {
				const wchar_t* itemPath = relativePath.c_str();

				if (relativePath.extension() == ".png" || relativePath.extension() == ".jpg" || relativePath.extension() == ".jpeg" || relativePath.extension() == ".wav" || relativePath.extension() == ".mp3" || relativePath.extension() == ".ttf")
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				if (relativePath.extension() == ".lua")
					ImGui::SetDragDropPayload("CONTENT_BROWSER_PREFAB", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));

				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
				if (directoryEntry.is_directory())
				{
					m_CurrentDirectory /= path.filename();
					m_TextureFolderContentHolder.clear();
				}
            }

			ImGui::TextWrapped(filenameString.c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::PopStyleVar();

		ImGui::End();
	}
}