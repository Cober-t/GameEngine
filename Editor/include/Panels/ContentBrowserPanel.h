#ifndef CONTENT_BROWSER_PANEL_H
#define CONTENT_BROWSER_PANEL_H

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
		std::filesystem::path m_CurrentDirectory;
		//Ref<Texture2D> m_DirectoryIcon;
		//Ref<Texture2D> m_FileIcon;
	};
}

#endif