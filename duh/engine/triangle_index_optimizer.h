// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstdint>
#include <cstddef>

namespace hg {

/**
	@short Optimize a list of triangle indices.

	The optimizer reorder the indices in order to maximize cache hits.
	You can specify the target architecture vertex cache size.

	@note This function can be called multiple times if needed.
*/
void OptimizeIndices(size_t idx_count, size_t vtx_count, const uint32_t *in_idx, uint32_t *out_idx, size_t cache_size = 32);

} // namespace hg
