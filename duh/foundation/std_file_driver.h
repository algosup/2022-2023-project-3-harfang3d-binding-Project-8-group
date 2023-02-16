// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/file_driver.h"
#include "foundation/string.h"

namespace hg {

/// Standard file driver
class StdFileDriver : public FileDriver {
public:
	StdFileDriver() = default;
	StdFileDriver(const std::string &root_path, bool sandbox = false);

	void SetRootPath(const std::string &path, bool sandbox = false);

	std::string MapToAbsolute(const std::string &path) const override;
	std::string MapToRelative(const std::string &path) const override;

	FileDriverCaps GetCaps() const override;

	std::shared_ptr<FileHandle> Open(const std::string &path, FileMode mode = FileRead) override;
	void Close(FileHandle &handle) override;

	bool Delete(const std::string &path) override;

	size_t Tell(FileHandle &handle) override;
	size_t Seek(FileHandle &handle, ptrdiff_t offset, SeekRef seek = SeekCurrent) override;
	size_t Size(FileHandle &handle) override;

	bool IsEOF(FileHandle &handle) override;

	size_t Read(FileHandle &handle, void *buffer_out, size_t size) override;
	size_t Write(FileHandle &handle, const void *buffer_in, size_t size) override;

	std::vector<DirEntry> Dir(const std::string &path, const std::string &wildcard, DirEntry::Type filter = DirEntry::File) override;
	bool MkDir(const std::string &path) override;
	bool IsDir(const std::string &path) override;
	std::shared_ptr<FileHandle> MkTempFile(const std::string &basename) override;
	std::string MkTempDir(const std::string &basename) override;

private:
	std::string root;
	bool sandbox{false};
};

} // namespace hg
