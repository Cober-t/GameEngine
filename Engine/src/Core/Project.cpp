#include <pch.h>
#include "Core/Project.h"

#include <fstream>
#include <nlohmann/json.hpp>

namespace Cober {

	std::optional<ProjectConfig> Project::Load(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			LOG_CORE_ERROR("Project file does not exist: {}", path.string());
			return std::nullopt;
		}

		std::ifstream file(path);
		if (!file.is_open())
		{
			LOG_CORE_ERROR("Could not open project file: {}", path.string());
			return std::nullopt;
		}

		nlohmann::json json;
		file >> json;

		ProjectConfig config;
		config.ProjectFilePath = std::filesystem::weakly_canonical(path);
		config.ProjectRoot = config.ProjectFilePath.parent_path();

		config.Name = json.value("name", "Cober");
		config.EngineVersion = json.value("engineVersion", "0.1.0");
		config.ProjectVersion = json.value("projectVersion", 1);
		config.StartupScene = json.value("startupScene", "assets/scenes/SceneDefault.lua");

		if (json.contains("window"))
		{
			auto& window = json["window"];
			config.WindowTitle = window.value("title", config.Name);
			config.WindowWidth = window.value("width", 1280);
			config.WindowHeight = window.value("height", 720);
		}

		if (json.contains("paths"))
		{
			auto& paths = json["paths"];
			config.AssetsPath = paths.value("assets", "assets");
		}

		if (!config.AssetsPath.is_absolute())
			config.AssetsPath = config.ProjectRoot / config.AssetsPath;

		if (!config.StartupScene.is_absolute())
			config.StartupScene = config.ProjectRoot / config.StartupScene;

		if (!std::filesystem::exists(config.AssetsPath))
		{
			LOG_CORE_ERROR("Assets path does not exist: {}", config.AssetsPath.string());
			return std::nullopt;
		}

		if (!std::filesystem::exists(config.StartupScene))
		{
			LOG_CORE_ERROR("Startup scene does not exist: {}", config.StartupScene.string());
			return std::nullopt;
		}

		return config;
	}
}