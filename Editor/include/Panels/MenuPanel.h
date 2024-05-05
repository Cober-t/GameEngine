#ifndef MENU_PANEL_H
#define MENU_PANEL_H

#include <Engine.h>
#include "ImGui/ImFileBrowser.h" 


namespace Cober {

	class MenuPanel {
	public:
		MenuPanel();
		~MenuPanel();

		static MenuPanel& Get() { return *s_Instance; }

		void OnGuiRender(Ref<EditorCamera>& editorCamera, Ref<Scene>& activeScene, Ref<Scene>& editorScene, Entity& hoveredEntity);

		void OpenFileDialog(Ref<Scene>& activeScene, const std::filesystem::path& path);

		bool MustSnap();
		float SnapValue();

	private:
		void Resize(Ref<EditorCamera>& camera, int width, int height, bool ortho);

	private:
		bool m_Fullscreen   = false;
		bool m_GridMenuOpen = false;
		bool m_World2D	   = false;
		bool m_Snap3D	   = false;
		bool m_Snap2D	   = false;
		int m_Snap3DValue   = 1;
		int m_Snap2DValue   = 1;

		enum BUILD_OPTION { WINDOWS = 0, LINUX, WEB };
		enum SCREEN_SIZE  { VERY_LOW = 0, LOW, MID, HIGH, VERY_HIGH, ULTRA, N_RESOLUTIONS };

		const char* m_CurrentScreenSize = "1280x720";
		const char* m_BuildValues[3]	= { "Windows", "Linux", "Web" };	// Add paths to makefiles instead platform names

		const char* m_CurrentBuildOption = "Empty";
		const char* m_ScreenValues[6] = {
			{  "640 x 480"  },
			{  "800 x 400"  },
			{ "1280 x 720"  },
			{ "1366 x 768"  },
			{ "1920 x 1080" },
			{ "2560 x 1440" }
		};
		glm::vec2 m_VPSize[6] = { 
            {1, 30}, 
            {800, 400}, 
            {1280, 720}, 
            {1366, 768}, 
            {1920, 1080}, 
            {2560, 1440} 
        };

		ImGui::FileBrowser m_FileBrowser;
		std::string m_FilePath;
		enum MenuOptions { OPEN = 0, SAVE_AS };
		MenuOptions m_MenuFileOption;
        
	private:
		static MenuPanel* s_Instance;
	};
}

#endif