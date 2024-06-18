#ifndef FONT_H
#define FONT_H

#include <filesystem>

#include "Core/Core.h"
#include "Render/Texture.h"

namespace Cober {

	struct MSDFData;

	class CB_API Font
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

		const MSDFData* GetMSDFData() const { return m_Data; }
		const std::string GetFontName() const { return m_FontName; }
		Ref<Texture> GetAtlasTexture() const { return m_AtlasTexture; }
		std::filesystem::path GetFontPath() const { return m_FontPath; }
		static int GetFontLoadedCount() { return m_AllFontPaths.size(); }

		static Ref<Font> GetDefault();

	private:
		MSDFData* m_Data;
		std::filesystem::path m_FontPath;
		std::string m_FontName;
		Ref<Texture> m_AtlasTexture;
		static std::unordered_map<std::string, std::pair<Ref<Texture>, MSDFData*>> m_AllFontPaths;
	};
}

#endif