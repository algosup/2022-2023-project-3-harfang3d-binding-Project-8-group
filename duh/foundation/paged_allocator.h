// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <mutex>
#include <vector>

namespace hg {

/// Memory page belonging to a memory allocator
template <size_t BlockSize, uint16_t PageSize>
struct AllocatorPage {
	AllocatorPage() {
		for (uint16_t i = 0; i < PageSize; ++i)
			*reinterpret_cast<uint16_t *>(data + BlockSize * i) = i + 1;
	}

	/// Test if the page is empty.
	inline bool IsEmpty() const { return usage == 0; }
	/// Test if the page is full.
	inline bool IsFull() const { return usage == PageSize; }
	/// Test if the page owns a pointer.
	inline bool Owns(const void *p) const { return p >= data && p < (data + BlockSize * PageSize); }

	inline void *Alloc(size_t size) {
		if (size > BlockSize)
			return nullptr;
		++usage;
		uint16_t grab = next;
		next = *reinterpret_cast<uint16_t *>(data + BlockSize * next);
		return data + BlockSize * grab;
	}

	inline void Free(void *p) {
		--usage;
		uint16_t grab = (reinterpret_cast<uint8_t *>(p) - data) / BlockSize;
		*reinterpret_cast<uint16_t *>(data + BlockSize * grab) = next;
		next = grab;
	}

	uint8_t data[BlockSize * PageSize];

	uint16_t next{0}; // next block index
	uint16_t usage{0};
};

/// Paged memory allocator
template <size_t BlockSize, uint16_t PageSize>
class PagedAllocator {
	typedef AllocatorPage<BlockSize, PageSize> Page;

public:
	PagedAllocator() {
		pages.reserve(8);
		NewPage();
	}

	void *Alloc(size_t size) {
		std::lock_guard<std::mutex> guard(lock);
		void *p = free_page->Alloc(size);

		if (free_page->IsFull()) {
			free_page = nullptr;
			for (auto page : pages)
				if (!page->IsFull()) {
					free_page = page;
					break;
				}

			if (!free_page)
				NewPage();
		}

		return p;
	}

	void Free(void *p) {
		std::lock_guard<std::mutex> guard(lock);
		for (auto i = pages.begin(); i != pages.end(); ++i) {
			auto page = *i;
			if (page->Owns(p)) {
				page->Free(p);

				if (page->IsEmpty() && page != free_page)
					pages.erase(i);
				break;
			}
		}
	}

private:
	std::mutex lock;

	void NewPage() { pages.push_back(free_page = new Page); }

	std::vector<Page *> pages;
	Page *free_page{nullptr};
};

} // namespace hg
