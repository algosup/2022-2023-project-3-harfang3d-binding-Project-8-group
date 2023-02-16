// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/small_block_allocator.h"

namespace hg {

/**
	@short Mixed block allocator
	Defer allocations to a pool of small block allocators.
*/
template <int PoolCount = 4, class Allocator = DefaultAllocator>
class MixedBlockAllocator {
public:
	void *malloc(size_t size) {
		void *p = nullptr;

		size_t pool_alloc_size = 8;
		for (int n = 0; n < PoolCount; ++n) {
			if (size <= pool_alloc_size) {
				p = sba[n].malloc(size);
				break;
			}
			pool_alloc_size *= 2;
		}

		return p ? p : Allocator::malloc(size);
	}

	void free(void *p) {
		for (int n = 0; n < PoolCount; ++n)
			if (sba[n].owns(p)) {
				sba[n].free(p);
				return;
			}

		Allocator::free(p);
	}

	void init(int profile[PoolCount]) { // ex: int profile[] = {16000, 16000, 8000, 8000} (usage: 128KB, 256KB, 256KB, 512KB)
		size_t pool_alloc_size = 8;
		for (int n = 0; n < PoolCount; ++n) {
			sba[n].init(pool_alloc_size, profile[n]);
			pool_alloc_size *= 2;
		}
	}

private:
	SmallBlockAllocator<Allocator> sba[PoolCount];
};

} // namespace hg
