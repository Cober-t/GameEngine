#include <pch.h>
#include "Panels/ContentBrowserPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Cober {

	ContentBrowserPanel* ContentBrowserPanel::s_Instance = nullptr;

	ContentBrowserPanel::ContentBrowserPanel()
	{
		s_Instance = this;
		m_AssetsPath = std::filesystem::current_path() /  "assets";
		m_CurrentDirectory = m_AssetsPath;

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


	void ContentBrowserPanel::OnGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != m_AssetsPath)
		{
			if (ImGui::ImageButton((ImTextureID)m_AssetIconMap["backwards"]->GetRendererID(), ImVec2(18.0f, 18.0f), { 0, 1 }, { 1, 0 }))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path().string() + "\\";
				m_TextureFolderContentHolder.clear();
			}
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Export global colors
			ImGui::Image((ImTextureID)m_AssetIconMap["backwards"]->GetRendererID(), ImVec2(18.0f, 18.0f), { 0, 1 }, { 1, 0 });
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
				ImGui::ImageButton((ImTextureID)m_AssetIconMap["folder"]->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}
			else
			{
				// Thumnails (Make an enum for file types)
				if (relativePath.extension() == ".png" || relativePath.extension() == ".jpg")
				{
					std::filesystem::path texturePath = m_CurrentDirectory / filenameString;
					if (m_TextureFolderContentHolder.find(texturePath.string()) == m_TextureFolderContentHolder.end())
					{
						m_TextureFolderContentHolder[texturePath.string()] = Texture::Create(texturePath.string());
					}
					ImGui::ImageButton((ImTextureID)m_TextureFolderContentHolder[texturePath.string()]->GetRendererID(), 
						{ thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				}
				else
					ImGui::ImageButton((ImTextureID)m_AssetIconMap["file"]->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}

			// Drag and Drop
			if (ImGui::BeginDragDropSource()) 
            {
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
            }

			ImGui::TextWrapped(filenameString.c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::PopStyleVar();

		ImGui::End();
	}
}