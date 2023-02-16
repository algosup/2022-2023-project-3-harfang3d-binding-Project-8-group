// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/dir_entry.h"
#include "foundation/file_mode.h"
#include "foundation/seek_ref.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace hg {

class FileHandle;

/// Provides access to named files on a file driver
class FileDriver : public std::enable_shared_from_this<FileDriver> {
public:
	virtual ~FileDriver() {}

	virtual std::string FileHash(const std::string &path);

	virtual std::string MapToAbsolute(const std::string &path) const { return path; }
	virtual std::string MapToRelative(const std::string &path) const { return path; }

	virtual FileDriverCaps GetCaps() const = 0;

	virtual std::shared_ptr<FileHandle> Open(const std::string &path, FileMode mode = FileRead) = 0;
	virtual void Close(FileHandle &handle) = 0;

	virtual bool Delete(const std::string &path) = 0;

	virtual size_t Tell(FileHandle &handle) = 0;
	virtual size_t Seek(FileHandle &handle, ptrdiff_t offset, SeekRef seek = SeekCurrent) = 0;
	virtual size_t Size(FileHandle &handle) = 0;

	virtual bool IsEOF(FileHandle &handle) = 0;

	virtual size_t Read(FileHandle &handle, void *buffer_out, size_t size) = 0;
	virtual size_t Write(FileHandle &handle, const void *buffer_in, size_t size) = 0;

	virtual std::vector<DirEntry> Dir(const std::string &path, const std::string &wildcard = "*.*", DirEntry::Type filter = DirEntry::All);
	virtual bool MkDir(const std::string &path) = 0;
	virtual bool IsDir(const std::string &path);
	virtual std::shared_ptr<FileHandle> MkTempFile(const std::string &/*path*/) { return {}; }
	virtual std::string MkTempDir(const std::string &/*path*/) { return {}; }
};

/// Return the content of a file as a string.
std::string FileToString(FileDriver &driver, const std::string &path);
/// Test if a file exists on the driver.
bool FileExists(FileDriver &driver, const std::string &path);

/// Return the content of a file as a byte buffer.
bool FileLoad(FileDriver &driver, const std::string &path, std::vector<char> &buffer);
/// Save a byte buffer to a file.
bool FileSave(FileDriver &driver, const std::string &path, const std::vector<char> &buffer);

/// Test if a file or directory exists on a driver.
bool Exists(FileDriver &driver, const std::string &path);

} // namespace hg
