// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/file_driver.h"

namespace hg {

/**
	@short Buffered I/O filesystem wrapper
	Implements a transparent read/write buffer on top of another filesystem.
*/
class BufferFileDriver : public FileDriver {
public:
	BufferFileDriver(std::shared_ptr<FileDriver> driver, size_t read_buffer_size = 4096, size_t write_buffer_size = 4096);

	FileDriverCaps GetCaps() const override;

	std::shared_ptr<FileHandle> Open(const std::string &path, FileMode mode = FileRead) override;
	void Close(FileHandle &handle) override;

	std::string FileHash(const std::string &path) override;
	bool Delete(const std::string &path) override;

	size_t Tell(FileHandle &handle) override;
	size_t Seek(FileHandle &handle, ptrdiff_t offset, SeekRef = SeekCurrent) override;
	size_t Size(FileHandle &handle) override;

	bool IsEOF(FileHandle &handle) override;

	size_t Read(FileHandle &handle, void *, size_t) override;
	size_t Write(FileHandle &handle, const void *, size_t) override;

	bool MkDir(const std::string &path) override;

private:
	std::shared_ptr<FileDriver> driver;
	size_t read_buffer_size, write_buffer_size;
};

} // namespace hg
