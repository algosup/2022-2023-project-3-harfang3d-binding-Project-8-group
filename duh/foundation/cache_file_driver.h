// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/data_store.h"
#include "foundation/file_driver.h"
#include "foundation/time.h"
#include <vector>
#include <memory>

namespace hg {

/**
	@short Cached I/O filesystem wrapper
	Implements a transparent file cache on top of another filesystem.
*/
class CacheFileDriver : public FileDriver {
public:
	CacheFileDriver(std::shared_ptr<FileDriver> driver, std::shared_ptr<FileDriver> store, size_t cache_size = MB(32));

	FileDriverCaps GetCaps() const override;

	std::shared_ptr<FileHandle> Open(const std::string &, FileMode mode = FileRead) override;
	void Close(FileHandle &handle) override;

	bool Delete(const std::string &) override;

	size_t Tell(FileHandle &handle) override;
	size_t Seek(FileHandle &handle, ptrdiff_t offset, SeekRef seek_ref = SeekCurrent) override;
	size_t Size(FileHandle &handle) override;

	bool IsEOF(FileHandle &handle) override;

	size_t Read(FileHandle &handle, void *, size_t size) override;
	size_t Write(FileHandle &handle, const void *, size_t size) override;

	bool MkDir(const std::string &path) override;

	/// Test if a given path is in cache.
	bool IsInCache(const std::string &path) { return GetCacheEntry(path) != entries.end(); }

	/// Reload cache state from its store.
	bool SynchronizeWithStore(const std::string &store_path = "store.db");

private:
	std::shared_ptr<FileDriver> driver;
	IODataStore store;

	std::mutex cache_mutex;

	struct Entry {
		std::string path, id;
		std::string hash;

		uint32_t refc{0};
		size_t size{0};

		time_ns last_use;
	};

	typedef std::vector<Entry> EntryList;

	EntryList entries;

	std::string ReserveOnStore(size_t size);

	EntryList::iterator GetCacheEntry(const std::string &path);
	EntryList::iterator CreateCacheEntry(const std::string &path);
	bool UpdateCacheEntry(Entry &entry, std::vector<char> *preloaded_data = nullptr);
	bool DeleteCacheEntry(const EntryList::iterator &i);

	friend struct FreeEntry;
};

} // namespace hg
