#ifndef EDITOR_RESOURCES_H
#define EDITOR_RESOURCES_H

#include <Engine.h>
#include <filesystem>

namespace Cober {

    class EditorResources
    {
    public:
        // General
		inline static Ref<Texture> CloseIcon = nullptr;
        inline static Ref<Texture> CompileIcon = nullptr;
		inline static Ref<Texture> GearIcon = nullptr;
        inline static Ref<Texture> LoadIcon = nullptr;
        inline static Ref<Texture> RefreshIcon = nullptr;
		inline static Ref<Texture> SearchIcon = nullptr;
		inline static Ref<Texture> SaveIcon = nullptr;

        // Console
		inline static Ref<Texture> ClearIcon = nullptr;
		inline static Ref<Texture> TraceMessagesIcon = nullptr;
        inline static Ref<Texture> InfoMessagesIcon = nullptr;
        inline static Ref<Texture> WarningMessagesIcon = nullptr;
        inline static Ref<Texture> ErrorMessagesIcon = nullptr;
        inline static Ref<Texture> EngineMessagesIcon = nullptr;
        inline static Ref<Texture> AppMessagesIcon = nullptr;

		// Scene Components
		inline static Ref<Texture> AssetIcon = nullptr;
		inline static Ref<Texture> AudioIcon = nullptr;
		inline static Ref<Texture> AudioListenerIcon = nullptr;
		inline static Ref<Texture> BoxCollider2DIcon = nullptr;
		inline static Ref<Texture> CameraIcon = nullptr;
		inline static Ref<Texture> CircleCollider2DIcon = nullptr;
		inline static Ref<Texture> RigidBodyIcon = nullptr;
		inline static Ref<Texture> RigidBody2DIcon = nullptr;
		inline static Ref<Texture> NativeScriptIcon = nullptr;
		inline static Ref<Texture> SpriteIcon = nullptr;
		inline static Ref<Texture> TextIcon = nullptr;
		inline static Ref<Texture> TransformIcon = nullptr;

		// Viewport
        inline static Ref<Texture> ForwardIcon = nullptr;
		inline static Ref<Texture> BackIcon = nullptr;
		inline static Ref<Texture> PlayIcon = nullptr;
		inline static Ref<Texture> PauseIcon = nullptr;
		inline static Ref<Texture> StopIcon = nullptr;
		inline static Ref<Texture> SimulateIcon = nullptr;
		inline static Ref<Texture> MoveIcon = nullptr;
		inline static Ref<Texture> RotateIcon = nullptr;
		inline static Ref<Texture> ScaleIcon = nullptr;

		// Window
		inline static Ref<Texture> MinimizeIcon = nullptr;
		inline static Ref<Texture> MaximizeIcon = nullptr;
		inline static Ref<Texture> RestoreIcon = nullptr;

		// Content Browser
        inline static Ref<Texture> EntityIcon = nullptr;
		inline static Ref<Texture> FolderIcon = nullptr;
		inline static Ref<Texture> FileIcon = nullptr;
        inline static Ref<Texture> BackwardsIcon = nullptr;
		inline static Ref<Texture> FBXFileIcon = nullptr;
		inline static Ref<Texture> OBJFileIcon = nullptr;
		inline static Ref<Texture> GLTFFileIcon = nullptr;
		inline static Ref<Texture> GLBFileIcon = nullptr;
		inline static Ref<Texture> WAVFileIcon = nullptr;
		inline static Ref<Texture> MP3FileIcon = nullptr;
		inline static Ref<Texture> PNGFileIcon = nullptr;
		inline static Ref<Texture> JPGFileIcon = nullptr;
		inline static Ref<Texture> SceneFileIcon = nullptr;
		inline static Ref<Texture> FontFileIcon = nullptr;

		// Textures
		inline static Ref<Texture> EngineLogoTexture = nullptr;
		inline static Ref<Texture> CheckerboardTexture = nullptr;


        static void Init()
        {
            // General
            CloseIcon = LoadTexture("Menu\\closeRemove.png", "CloseRemove");
            CompileIcon = LoadTexture("Menu\\compile.png", "Compile");
            GearIcon = LoadTexture("Menu\\settings.png", "Settings");
            LoadIcon = LoadTexture("Menu\\load.png", "Load");
            RefreshIcon = LoadTexture("Menu\\refresh.png", "Refresh");
            SearchIcon = LoadTexture("Menu\\search.png", "Search");
            SaveIcon = LoadTexture("Menu\\save.png", "Save");

            // Console
            AppMessagesIcon = LoadTexture("Console\\appFilter.png", "AppFilter");
            ClearIcon = LoadTexture("Console\\clear.png", "ClearMessages");
            EngineMessagesIcon = LoadTexture("Console\\engineFilter.png", "EngineFilter");
            ErrorMessagesIcon = LoadTexture("Console\\errorMessages.png", "ErrorMessages");
            InfoMessagesIcon = LoadTexture("Console\\infoMessages.png", "InfoMessages");
            TraceMessagesIcon = LoadTexture("Console\\traceMessages.png", "TraceMessages");
            WarningMessagesIcon = LoadTexture("Console\\warningMessages.png", "WarningMessages");

            // Scene Components
            // AssetIcon = LoadTexture("RelativeFolder/iconName.png", "PlaceholderTextureName");
            AudioIcon = LoadTexture("SceneComponents\\audio.png", "Audio");
            // AudioListenerIcon = LoadTexture("RelativeFolder/iconName.png", "PlaceholderTextureName");
            BoxCollider2DIcon = LoadTexture("SceneComponents\\boxCollider2D.png", "BoxCollider2D");
            CameraIcon = LoadTexture("SceneComponents\\camera.png", "Camera");
            CircleCollider2DIcon = LoadTexture("SceneComponents\\circleCollider2D.png", "CircleCollider2D");
            RigidBody2DIcon = LoadTexture("SceneComponents\\rigidbody2D.png", "Rigidbody2D");
            NativeScriptIcon = LoadTexture("SceneComponents\\nativeScript.png", "NativeScript");
            SpriteIcon = LoadTexture("SceneComponents\\sprite.png", "Sprite");
            TextIcon = LoadTexture("SceneComponents\\text.png", "Text");
            TransformIcon = LoadTexture("SceneComponents\\transform.png", "TransformIcon");

            // Viewport
            BackIcon = LoadTexture("Viewport\\back.png", "Back");
            ForwardIcon = LoadTexture("Viewport\\forward.png", "Forward");
            PlayIcon = LoadTexture("Viewport\\play.png", "Play");
            PauseIcon = LoadTexture("Viewport\\pause.png", "Pause");
            StopIcon = LoadTexture("Viewport\\stop.png", "Stop");

            // Window
            // MinimizeIcon = LoadTexture("RelativeFolder/iconName.png", "PlaceholderTextureName");
            // MaximizeIcon = LoadTexture("RelativeFolder/iconName.png", "PlaceholderTextureName");
            // RestoreIcon = LoadTexture("RelativeFolder/iconName.png", "PlaceholderTextureName");

            // Content Browser
            EntityIcon = LoadTexture("SceneComponents\\entity.png", "Entity");
            FolderIcon = LoadTexture("ContentBrowser\\folder.png", "Folder");
            FileIcon = LoadTexture("ContentBrowser\\file.png", "File");
            BackwardsIcon = LoadTexture("ContentBrowser\\backwards.png", "Backwards");
            // PNGFileIcon = LoadTexture("RelativeFolder/iconName.png", "PlaceholderTextureName");
            // JPGFileIcon = LoadTexture("RelativeFolder/iconName.png", "PlaceholderTextureName");
            SceneFileIcon = LoadTexture("ContentBrowser\\scene.png", "Scene");

            // Textures
            // EngineLogoTexture = LoadTexture("RelativeFolder/iconName.png", "PlaceholderTextureName");
            // CheckerboardTexture = LoadTexture("RelativeFolder/iconName.png", "PlaceholderTextureName");
        }


        static void Shutdown()
        {
            // General
            CloseIcon.reset();
            CompileIcon.reset();
            GearIcon.reset();
            LoadIcon.reset();
            RefreshIcon.reset();
            SearchIcon.reset();
            SaveIcon.reset();

            // Console
            AppMessagesIcon.reset();
            ClearIcon.reset();
            EngineMessagesIcon.reset();
            ErrorMessagesIcon.reset();
            InfoMessagesIcon.reset();
            TraceMessagesIcon.reset();
            WarningMessagesIcon.reset();

            // Scene Components
            // AssetIcon.reset();
            AudioIcon.reset();
            // AudioListenerIcon.reset();
            BoxCollider2DIcon.reset();
            CameraIcon.reset();
            CircleCollider2DIcon.reset();
            RigidBody2DIcon.reset();
            NativeScriptIcon.reset();
            SpriteIcon.reset();
            TextIcon.reset();

            // Viewport
            BackIcon.reset();
            ForwardIcon.reset();
            PlayIcon.reset();
            PauseIcon.reset();
            StopIcon.reset();

            // Content Browser
            EntityIcon.reset();
            FolderIcon.reset();
            FileIcon.reset();
            // PNGFileIcon.Reset();
            // JPGFileIcon.Reset();
            SceneFileIcon.reset();

            // Textures
            // EngineLogoTexture.Reset();
            // CheckerboardTexture.Reset();
        }

    private:

		static Ref<Texture> LoadTexture(const std::filesystem::path& relativePath, const std::string& name)
		{
			std::filesystem::path path = std::filesystem::path(EDITOR_ICONS_DIR) / relativePath;
            
			if (!std::filesystem::exists(std::filesystem::path(path)))
			{
				LOG_CORE_ERROR("Failed to load icon {0}. The file doesn't exist.", path.string());
				return nullptr;
			}

            // Provisional until Create Textures from a path
			return Texture::Create(path.string());
		}
    };
}


#endif