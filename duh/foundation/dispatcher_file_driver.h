// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/file_driver.h"

namespace hg {

/// Dispatcher file driver
class DispatcherFileDriver : public FileDriver {
public:
	void AddDispatch(std::shared_ptr<FileDriver> driver, const std::string &prefix = std::string());

	FileDriverCaps GetCaps() const override;

	std::shared_ptr<FileHandle> Open(const std::string &path, FileMode mode = FileRead) override;
	void Close(FileHandle &handle) override;

	std::string FileHash(const std::string &path) override;
	bool Delete(const std::string &path) override;

	size_t Tell(FileHandle &handle) override;
	size_t Seek(FileHandle &handle, ptrdiff_t offset, SeekRef seek_ref = SeekCurrent) override;

	size_t Read(FileHandle &handle, void *data, size_t size) override;
	size_t Write(FileHandle &handle, const void *data, size_t size) override;

	bool MkDir(const std::string &path) override;

private:
	struct DispatchFS {
		std::shared_ptr<FileDriver> driver;
		std::string prefix;
	};

	std::vector<std::shared_ptr<FileDriver>> roots;
	std::vector<DispatchFS> mounts;

	std::shared_ptr<FileDriver> Dispatch(std::string &path, FileMode mode);
};

} // namespace hg
