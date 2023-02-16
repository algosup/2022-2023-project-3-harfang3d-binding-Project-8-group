// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/allocator.h"
#include <cassert>
#include <mutex>

namespace hg {

template <class Allocator = DefaultAllocator>
class SmallBlockAllocator {
public:
	~SmallBlockAllocator() { uninit(); }

	inline bool owns(void *p) const {
		std::lock_guard<std::mutex> guard(lock);
		return p >= pool && p < pool_end;
	}

	void *malloc(size_t size) {
		std::lock_guard<std::mutex> guard(lock);

		if (size > block_size || !root)
			return Allocator::malloc(size);

		void *p = root;
		root = *((void **)root);
		return p;
	}

	void free(void *p) {
		std::lock_guard<std::mutex> guard(lock);

		if (p < pool || p >= pool_end) {
			Allocator::free(p);
		} else {
			*((void **)p) = root;
			root = p;
		}
	}

	bool init(size_t block_size_, uint32_t block_count) {
		__ASSERT__(pool == nullptr);

		std::lock_guard<std::mutex> guard(lock);

		pool = Allocator::malloc(block_size_ * block_count);
		if (!pool)
			return false;
		pool_end = reinterpret_cast<char *>(pool) + block_size_ * block_count;

		for (uint32_t n = 0; n < block_count - 1; ++n)
			*((void **)(reinterpret_cast<char *>(pool) + n * block_size_)) = reinterpret_cast<char *>(pool) + (n + 1) * block_size_;
		*((void **)(reinterpret_cast<char *>(pool) + (block_count - 1) * block_size_)) = nullptr;

		root = pool;
		block_size = block_size_;
		return true;
	}

	void uninit() {
		std::lock_guard<std::mutex> guard(lock);

		Allocator::free(pool);
		pool = nullptr;
		pool_end = nullptr;
		root = nullptr;
		block_size = 0;
	}

private:
	mutable std::mutex lock;

	size_t block_size{0};

	void *root{nullptr};
	void *pool{nullptr}, *pool_end{nullptr};
};

} // namespace hg
