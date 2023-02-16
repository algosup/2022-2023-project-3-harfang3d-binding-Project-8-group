// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/allocator_profiler.h"

namespace hg {

/// Default C heap allocator.
struct DefaultHeapAllocator {
	static void *malloc(size_t size);
	static void free(void *p);
};

// Default to the C heap allocator.
#if 0 // _DEBUG

/*
	Note: Using the allocator profiler on the default and global heap allocator
	is not working with managed containers such as auto_list. These container
	ownership policy use a single allocator to free memory and will fail if
	a different allocator was used to allocate the managed object.
*/
struct DefaultHeapAllocatorProfiler {
	static singleton<AllocatorProfiler<DefaultHeapAllocator> > allocator;
	static void *malloc(size_t size) { return allocator->malloc(size); }
	static void free(void *p) { allocator->free(p); }
};

using DefaultAllocator =DefaultHeapAllocatorProfiler;
#else
using DefaultAllocator = DefaultHeapAllocator;
#endif

//
#if defined(EMSCRIPTEN)
#define UseAllocator(ALLOCATOR)
#define UseAllocatorEmplace(TYPE, ALLOCATOR)
#else
#define UseAllocator(ALLOCATOR)                                                  \
	inline void *operator new(size_t size) { return ALLOCATOR::malloc(size); }   \
	inline void operator delete(void *addr) { ALLOCATOR::free(addr); }           \
	inline void *operator new[](size_t size) { return ALLOCATOR::malloc(size); } \
	inline void operator delete[](void *addr) { ALLOCATOR::free(addr); }

#define UseAllocatorEmplace(TYPE, ALLOCATOR)                                           \
	inline void *operator new(size_t size, void *addr) { return ::new (addr) TYPE(); } \
	UseAllocator(ALLOCATOR)
#endif

//
template <class T>
void _safe_delete(T *&p) {
	delete p;
	p = nullptr;
}

template <class T>
void _safe_delete_array(T *&p) {
	delete[] p;
	p = nullptr;
}

} // namespace hg
