// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/unit.h"
#include <mutex>
#include <memory>
#include <string>
#include <vector>

namespace hg {

class FileDriver;

/// Data store
class IODataStore {
public:
	struct Entry {
		std::string id;
		std::string user;
		size_t size{0};
	};

	IODataStore(std::shared_ptr<FileDriver> storage, size_t storage_limit = MB(32)) : driver(storage), limit(storage_limit) { entries.reserve(64); }

	const std::shared_ptr<FileDriver> &GetDriver() const { return driver; }

	const std::vector<Entry> &GetEntries() const { return entries; }

	/// Reserve space on the store, an empty id is returned if the store is full.
	virtual std::string Reserve(size_t size);
	/// Free a store alias.
	virtual bool Free(const std::string &id);

	/// Store data on a reserved id.
	bool Store(const std::string &id, const void *data, size_t size, const std::string &user_data = std::string());

	/// Get entry size.
	size_t GetEntrySize(const std::string &id);

	/// Get the current store size.
	size_t GetStoreSize() const;
	/// Get the store free space size.
	size_t GetFreeStore() const;

	/// Restore store content from the storage fs.
	bool Load(const std::string &path = "store.db");
	/// Save store content to the storage fs.
	bool Save(const std::string &path = "store.db");

private:
	std::mutex access;

	std::shared_ptr<FileDriver> driver;
	size_t limit;

	uint32_t id_seed{0};
	std::string GetNewId();

	std::vector<Entry> entries;
	std::vector<Entry>::iterator GetEntry(const std::string &id);
};

} // namespace hg
