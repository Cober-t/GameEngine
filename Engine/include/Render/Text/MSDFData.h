#ifndef MSDF_DATA_H
#define MSDF_DATA_H

#include <vector>

#undef INFINITE
#include "msdf-atlas-gen.h"

namespace Cober {

	struct MSDFData
	{
		std::vector<msdf_atlas::GlyphGeometry> Glyphs;
		msdf_atlas::FontGeometry FontGeometry;
	};
}

#endif