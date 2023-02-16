// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/node.h"
#include "foundation/matrix4.h"

namespace hg {

struct Skeleton {
	std::vector<std::shared_ptr<Node>> bones;
	std::vector<Matrix4> bones_mtx, previous_bones_mtx;
};

} // namespace hg
