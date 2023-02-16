// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/time.h"
#include <cstddef>

namespace hg {

struct RenderStatistics {
	RenderStatistics();

	void Reset();

	time_ns frame_start;
	size_t render_primitive_drawn, line_drawn, triangle_drawn, instanced_batch_count, instanced_batch_size;
};

} // namespace hg
