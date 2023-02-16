// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/memory_file_driver.h"

namespace hg {

/// Crypto file driver
class CryptoFileDriver : public FileDriver {
public:
	CryptoFileDriver(const std::shared_ptr<FileDriver> &driver, const std::string &key);

	FileDriverCaps GetCaps() const override;

	std::shared_ptr<FileHandle> Open(const std::string &path, FileMode mode = FileRead) override;
	void Close(FileHandle &handle) override;

	bool Delete(const std::string &path) override;

	size_t Tell(FileHandle &handle) override;
	size_t Seek(FileHandle &handle, ptrdiff_t offset, SeekRef ref = SeekCurrent) override;

	size_t Read(FileHandle &handle, void *out, size_t size) override;
	size_t Write(FileHandle &handle, const void *in, size_t size) override;

	bool MkDir(const std::string &path) override;

private:
	std::string key;

	std::shared_ptr<FileDriver> wrapped_io;
	std::shared_ptr<MemoryFileDriver> cache_driver;

	void Encrypt(std::vector<char> &data);
	void Decrypt(std::vector<char> &data);
};

} // namespace hg
