#ifndef FONT_H
#define FONT_H

#include <filesystem>

#include "Core/Core.h"
#include "Render/Texture.h"

namespace Cober {

	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

		const MSDFData* GetMSDFData() const { return m_Data; }
		Ref<Texture> GetAtlasTexture() const { return m_AtlasTexture; }

		static Ref<Font> GetDefault();
	private:
		MSDFData* m_Data;
		Ref<Texture> m_AtlasTexture;
	};
}

#endif