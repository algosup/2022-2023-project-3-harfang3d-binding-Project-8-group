// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstdint>
#include <cstddef>

namespace hg {

/// Decorator class to track an allocator key metrics
template <class Allocator>
struct AllocatorProfiler {
	struct Profile {
		uint32_t alloc_count{0};
		uint32_t alive_count{0}, alive_count_peak{0};
		size_t size{0}, size_peak{0};
	};

	void *malloc(size_t size) {
		auto h = reinterpret_cast<Header *>(Allocator::malloc(sizeof(Header) + size));
		h->size = size;

		++profile.alloc_count;
		++profile.alive_count;
		if (profile.alive_count > profile.alive_count_peak)
			profile.alive_count_peak = profile.alive_count;

		profile.size += size;
		if (profile.size > profile.size_peak)
			profile.size_peak = profile.size;

		return h + 1;
	}

	void free(void *p) {
		if (p) {
			auto h = reinterpret_cast<Header *>(p) - 1;
			profile.size -= h->size;

			Allocator::free(h);
			--profile.alive_count;
		}
	}

	const Profile &get_profile() const { return profile; }

private:
	struct Header {
		size_t size{0};
	};

	Profile profile;
};

} // namespace hg
