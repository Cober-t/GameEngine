#ifndef MENU_PANEL_H
#define MENU_PANEL_H

#include <Engine.h>
#include "ImGui/ImFileBrowser.h" 


namespace Cober {

	enum MenuOptions { SAVE = 0, LOAD, COMPILE };
	enum BuildOptions { WINDOWS = 0, LINUX, WEB };
	enum ScreenSize  { VERY_LOW = 0, LOW, MID, HIGH, VERY_HIGH, ULTRA, N_RESOLUTIONS };

	struct Settings
	{
		bool Fullscreen   = false;
		bool Vsync   = false;
		bool GridMenuOpen = false;
		bool World2D	   = false;
		bool Snap3D	   = false;
		bool Snap2D	   = false;
		int Snap3DValue   = 1;
		int Snap2DValue   = 1;

		const char* CurrentScreenSize = "1280x720";
		const char* BuildValues[3]	= { "Windows", "Linux", "Web" };	// Add paths to makefiles instead platform names

		const char* CurrentBuildOption = "Empty";
		// const char* ScreenValues[6] = {
		// 	{  "640 x 480"  },
		// 	{  "800 x 400"  },
		// 	{ "1280 x 720"  },
		// 	{ "1366 x 768"  },
		// 	{ "1920 x 1080" },
		// 	{ "2560 x 1440" }
		// };
		// glm::vec2 VPSize[6] = { 
        //     {1, 30}, 
        //     {800, 400}, 
        //     {1280, 720}, 
        //     {1366, 768}, 
        //     {1920, 1080}, 
        //     {2560, 1440} 
        // };
	};

	
	class MenuPanel {
	public:
		MenuPanel();
		~MenuPanel();

		static MenuPanel& Get() { return *s_Instance; }

		void OnGuiRender(Ref<EditorCamera>& editorCamera);

		inline bool Snap() { return m_Settings.World2D == true ? m_Settings.Snap2D : m_Settings.Snap3D; }
		inline float GetSnapValue() { return m_Settings.World2D == true ? m_Settings.Snap2DValue : m_Settings.Snap3DValue; }

	private:
		void FileMenu(Ref<EditorCamera>& editorCamera);
		void OptionsMenu(Ref<EditorCamera>& editorCamera);
		void OpenFileDialog();

	private:
		// ImGui::FileBrowser* m_FileBrowser;
		std::string m_FilePath;
		std::filesystem::path m_SaveFile;
		std::filesystem::path m_LoadFile;

		std::filesystem::path m_BuildFileCommand;
		std::filesystem::path m_CompileFileCommand;
		std::filesystem::path m_GamePath;
		std::filesystem::path m_ScenesPath;

		Settings m_Settings;
		bool m_ShotFontAtlas;
        
	private:
		static MenuPanel* s_Instance;
	};
}

#endif