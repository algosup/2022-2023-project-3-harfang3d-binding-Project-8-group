// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/color.h"
#include "foundation/vector2.h"
#include "foundation/vector3.h"
#include <map>
#include <memory>
#include <vector>

namespace hg {

struct Texture;
class RenderSystem;

class RasterFontBatch {
public:
	void AddGlyph(const Vector3 vtxs[4], const Vector2 uvs[4], const Color rgbs[4], std::shared_ptr<Texture> page);
	void Draw(RenderSystem &render_system);

private:
	struct Batch {
		static const int reserve_glyph = 64;

		Batch() {
			vtxs.reserve(4 * reserve_glyph);
			uvs.reserve(4 * reserve_glyph);
			rgbs.reserve(4 * reserve_glyph);
		}

		void Clear() {
			vtxs.clear();
			uvs.clear();
			rgbs.clear();
		}

		std::vector<Vector3> vtxs;
		std::vector<Vector2> uvs;
		std::vector<Color> rgbs;
	};

	std::map<std::shared_ptr<Texture>, Batch> batches;
	std::vector<uint16_t> idxs;

	uint16_t *GetIndices(size_t glyph_count);
};

} // namespace hg
