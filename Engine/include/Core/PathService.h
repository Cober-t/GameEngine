#ifndef PATH_SERVICE_H
#define PATH_SERVICE_H

#include <filesystem>
#include "Core.h"

namespace Cober {
	
	class CB_API PathService
	{
		public:
		static void Init(const std::filesystem::path& projectRoot, const std::filesystem::path& assetsRoot);

		static const std::filesystem::path& ProjectRoot();
		static const std::filesystem::path& AssetsRoot();
		
		static std::filesystem::path ResolveProject(const std::filesystem::path& relativePath);
		static std::filesystem::path ResolveAsset(const std::filesystem::path& relativePath);

	private:
		inline static std::filesystem::path s_ProjectRoot;
		inline static std::filesystem::path s_AssetsRoot;
	};
}

#endif