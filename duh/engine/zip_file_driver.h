// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/memory_file_driver.h"
#include <map>
#include <memory>
#include <mutex>

namespace hg {

struct ZipEngine;
class FileHandle;

/// Zip-based I/O
class ZipFileDriver : public FileDriver {
public:
	typedef std::map<std::string, int> RefcMap;

	ZipFileDriver(std::shared_ptr<FileHandle> archive, const std::string &password = std::string());
	~ZipFileDriver();

	bool SetArchive(std::shared_ptr<FileHandle> archive, const std::string &password = std::string());

	FileDriverCaps GetCaps() const override;

	std::shared_ptr<FileHandle> Open(const std::string &path, FileMode mode = FileRead) override;
	void Close(FileHandle &handle) override;

	bool Delete(const std::string &path) override;

	size_t Tell(FileHandle &handle) override;
	size_t Seek(FileHandle &handle, ptrdiff_t offset, SeekRef seek_ref = SeekCurrent) override;
	size_t Size(FileHandle &handle) override;

	bool IsEOF(FileHandle &handle) override;

	size_t Read(FileHandle &handle, void *data, size_t size) override;
	size_t Write(FileHandle &handle, const void *data, size_t size) override;

	std::vector<DirEntry> Dir(const std::string &path, const std::string &wildcard, DirEntry::Type filter = DirEntry::All) override;
	bool MkDir(const std::string &path) override { return false; }

private:
	std::mutex zip_mutex;

	std::unique_ptr<ZipEngine> zip_engine;
	std::shared_ptr<FileHandle> archive;

	void *zfile;
	std::string password;

	RefcMap refc_map;
	std::shared_ptr<MemoryFileDriver> memory_driver; // for nested compressed zip
};

} // namespace hg
