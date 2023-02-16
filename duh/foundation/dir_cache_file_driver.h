// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <foundation/file_driver.h>
#include <map>

namespace hg {

class DirCacheFileDriver : public FileDriver {
public:
	DirCacheFileDriver(std::shared_ptr<FileDriver> _driver) : driver(_driver) {}

	std::string FileHash(const std::string &path) override { return driver->FileHash(path); }

	std::string MapToAbsolute(const std::string &path) const override { return driver->MapToAbsolute(path); }
	std::string MapToRelative(const std::string &path) const override { return driver->MapToRelative(path); }

	FileDriverCaps GetCaps() const override { return driver->GetCaps(); }

	std::shared_ptr<FileHandle> Open(const std::string &path, FileMode mode = FileRead) override { return driver->Open(path, mode); }
	void Close(FileHandle &handle) override { return driver->Close(handle); }

	bool Delete(const std::string &path) override { return driver->Delete(path); }

	size_t Tell(FileHandle &handle) override { return driver->Tell(handle); }
	size_t Seek(FileHandle &handle, ptrdiff_t offset, SeekRef seek = SeekCurrent) override { return driver->Seek(handle, offset, seek); }
	size_t Size(FileHandle &handle) override { return driver->Size(handle); }

	bool IsEOF(FileHandle &handle) override { return driver->IsEOF(handle); }

	size_t Read(FileHandle &handle, void *buffer_out, size_t size) override { return driver->Read(handle, buffer_out, size); }
	size_t Write(FileHandle &handle, const void *buffer_in, size_t size) override { return driver->Write(handle, buffer_in, size); }

	std::vector<DirEntry> Dir(const std::string &path, const std::string &wildcard = "*.*", DirEntry::Type filter = DirEntry::All) override;
	bool MkDir(const std::string &path) override { return driver->MkDir(path); }
	bool IsDir(const std::string &path) override { return driver->IsDir(path); }

	void InvalidateCache(const std::string &path);
	bool IsInCache(const std::string &path, DirEntry::Type filter = DirEntry::All);

private:
	std::shared_ptr<FileDriver> driver;
	std::map<DirEntry::Type, std::map<std::string, std::vector<DirEntry>>> dir_cache;
};

} // namespace hg
