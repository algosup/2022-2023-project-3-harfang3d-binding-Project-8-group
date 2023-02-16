// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/math.h"
#include "foundation/matrix4.h"
#include <memory>
#include <string>
#include <vector>

namespace hg {

class Geometry;

struct GeometryMergeEntry {
	std::shared_ptr<Geometry> geo;
	Matrix4 mtx;
};

std::shared_ptr<Geometry> GeometryMerge(const std::string &name, const std::vector<GeometryMergeEntry> &geos);

} // namespace hg
