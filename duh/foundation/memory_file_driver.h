// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/file_driver.h"
#include <memory>
#include <mutex>
#include <vector>

namespace hg {

class MemoryFile {
public:
	friend class MemoryFileDriver;

	inline const std::string &GetPath() const { return path; }
	inline const std::vector<char> &GetData() const { return data; }
	inline size_t GetSize() const { return size; }

private:
	std::string path;
	std::vector<char> data;
	size_t size; // might be != data.GetSize()

	explicit MemoryFile(const std::string &_path = nullptr) : path(_path), size(0) {}
};

typedef std::vector<std::shared_ptr<MemoryFile>> MemoryFat;

/// Memory I/O
class MemoryFileDriver : public FileDriver {
public:
	/// Return the system file allocation table.
	inline const MemoryFat &GetFat() const { return fat; }
	/// Return the system table of content.
	std::vector<std::string> GetToc() const;
	/// Return memory usage.
	size_t GetMemoryUsage() const;

	FileDriverCaps GetCaps() const override;

	std::shared_ptr<FileHandle> Open(const std::string &path, FileMode mode = FileRead) override;
	void Close(FileHandle &handle) override;

	bool Delete(const std::string &path) override;

	size_t Tell(FileHandle &handle) override;
	size_t Seek(FileHandle &handle, ptrdiff_t offset, SeekRef = SeekCurrent) override;
	size_t Size(FileHandle &handle) override;

	bool IsEOF(FileHandle &handle) override;

	size_t Read(FileHandle &handle, void *out, size_t size) override;
	size_t Write(FileHandle &handle, const void *in, size_t size) override;

	bool MkDir(const std::string &path) override;

private:
	mutable std::mutex fat_mutex;

	MemoryFat fat;
};

} // namespace hg
