#ifndef PROJECT_H
#define PROJECT_H

#include <filesystem>
#include <optional>
#include <string>

#include "Core.h"

namespace Cober {

	struct ProjectConfig
	{
		std::string Name = "Cober";
		std::string EngineVersion = "0.1.0";
		uint32_t ProjectVersion = 1;

		std::filesystem::path ProjectFilePath;
		std::filesystem::path ProjectRoot;

		std::filesystem::path AssetsPath = "assets";
		std::filesystem::path StartupScene = "assets/scenes/SceneDefault.lua";

		uint32_t WindowWidth = 1280;
		uint32_t WindowHeight = 720;
		std::string WindowTitle = "Cober";
	};

	class CB_API Project
	{
	public:
		static std::optional<ProjectConfig> Load(const std::filesystem::path& path);
	};
}

#endif