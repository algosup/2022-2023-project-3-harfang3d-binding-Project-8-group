// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/base_file_handle.h"
#include <mutex>
#include <memory>

namespace hg {

class FileDriver;

class FileHandle : public BaseFileHandle {
public:
	FileHandle() = delete;
	FileHandle(std::shared_ptr<FileDriver> driver_) : driver(driver_) {}

	size_t GetSize() override;

	size_t Rewind() override;
	bool IsEOF() override;

	size_t Tell() override;
	size_t Seek(ptrdiff_t offset, SeekRef seek_ref = SeekCurrent) override;

	size_t Read(void *out_data, size_t size) override;
	size_t Write(const void *in_data, size_t size) override;

	template <class T>
	bool Write(const T &v) { return BaseFileHandle::Write<T>(v); }
	template <class T>
	T Read() { return BaseFileHandle::Read<T>(); }

	void lock() { lock_.lock(); }
	void unlock() { lock_.unlock(); }

	std::shared_ptr<FileDriver> GetDriver() const { return driver; }

protected:
	std::recursive_mutex lock_;
	std::shared_ptr<FileDriver> driver;
};

} // namespace hg
