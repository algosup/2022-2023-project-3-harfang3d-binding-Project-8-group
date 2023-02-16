// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/color.h"
#include "foundation/vector2.h"
#include "foundation/vector3.h"
#include <memory>
#include <vector>

namespace hg {

struct Texture;

class TriangleBatch {
public:
	explicit TriangleBatch(size_t batch_max_tri = 128);

	void DrawTriangle(size_t triangle_count, size_t attrib_count, const Vector3 *vtx, const uint16_t *idx, const Color *col, const Vector2 *uv, const std::shared_ptr<Texture> &tex);
	void Flush();

private:
	size_t batch_max_triangle_count;
	size_t batch_triangle_count, batch_attrib_offset;

	std::vector<uint16_t> batch_idx;
	std::vector<Vector3> batch_vtx;
	std::vector<Color> batch_col;
	std::vector<Vector2> batch_uv;

	std::shared_ptr<Texture> batch_t;
};

} // namespace hg
