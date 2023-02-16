// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/file_handle.h"

namespace hg {

/// Maps a FileHandle segment (offset/size) to a new FileHandle
class FileHandleSegment : public FileHandle {
public:
	FileHandleSegment(FileHandle *h, size_t o, size_t s) : FileHandle(nullptr), handle(h), offset(o), size(s) {}

	size_t GetSize() override { return size; }

	size_t Tell() override { return cursor; }
	size_t Seek(ptrdiff_t offset, SeekRef seek_ref = SeekCurrent) override;

	size_t Read(void *p_out, size_t size) override;
	size_t Write(const void *ptr, size_t size) override;

private:
	FileHandle *handle;
	size_t offset, size;
	size_t cursor{0};
};

} // namespace hg
