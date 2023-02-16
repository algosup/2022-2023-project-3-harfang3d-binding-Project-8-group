// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/seek_ref.h"
#include <cstddef>

namespace hg {

/// Base file handle
class BaseFileHandle {
public:
	virtual ~BaseFileHandle() {}

	virtual size_t GetSize() = 0;

	virtual size_t Rewind() = 0;
	virtual bool IsEOF() = 0;

	virtual size_t Tell() = 0;
	virtual size_t Seek(ptrdiff_t offset, SeekRef ref = SeekCurrent) = 0;

	virtual size_t Read(void *out_data, size_t size) = 0;
	virtual size_t Write(const void *in_data, size_t size) = 0;

	template <class T>
	bool Write(const T &v) { return this->Write(&v, sizeof(T)) == sizeof(T); }

	template <class T>
	T Read() {
		T v;
		this->Read(&v, sizeof(T));
		return v;
	}
};

/// Stream operator to write a value to a handle.
template <class T>
BaseFileHandle &operator<<(BaseFileHandle &h, const T &v) {
	h.Write(&v, sizeof(T));
	return h;
}

/// Stream operator to read a value from a handle.
template <class T>
BaseFileHandle &operator>>(BaseFileHandle &h, T &v) {
	h.Read(&v, sizeof(T));
	return h;
}

} // namespace hg
