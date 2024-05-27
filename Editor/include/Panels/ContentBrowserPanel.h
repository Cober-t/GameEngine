#ifndef CONTENT_BROWSER_PANEL_H
#define CONTENT_BROWSER_PANEL_H

#include "Theme/EditorResources.h"
#include <filesystem>

namespace Cober {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel();

		static ContentBrowserPanel& Get() { return *s_Instance; }

		void OnGuiRender();
        
	private:
		static ContentBrowserPanel* s_Instance;
		std::filesystem::path m_AssetsPath;
		std::filesystem::path m_CurrentDirectory;
		std::map<std::string, Ref<Texture>> m_AssetIconMap;
		std::map<std::string, Ref<Texture>> m_TextureFolderContentHolder;
	};
}

#endif