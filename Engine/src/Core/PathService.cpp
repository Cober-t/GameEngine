#include <pch.h>
#include "Core/PathService.h"

namespace Cober {

	void PathService::Init(const std::filesystem::path& projectRoot, const std::filesystem::path& assetsRoot)
	{
		s_ProjectRoot = std::filesystem::weakly_canonical(projectRoot);
		s_AssetsRoot = std::filesystem::weakly_canonical(assetsRoot);

		LOG_CORE_INFO("Project Root: {}", projectRoot.string());
		LOG_CORE_INFO("Assets Root: {}", assetsRoot.string());
	}

	const std::filesystem::path& PathService::ProjectRoot()
	{
		return s_ProjectRoot;
	}

	const std::filesystem::path& PathService::AssetsRoot()
	{
		return s_AssetsRoot;
	}

    std::filesystem::path PathService::ResolveProject(const std::filesystem::path &relativePath)
    {
		if (relativePath.is_absolute())
			return relativePath;
		return s_ProjectRoot / relativePath;
	}

	std::filesystem::path PathService::ResolveAsset(const std::filesystem::path& relativePath)
	{
		if (relativePath.is_absolute())
			return relativePath;
		return s_AssetsRoot / relativePath;
	}
}