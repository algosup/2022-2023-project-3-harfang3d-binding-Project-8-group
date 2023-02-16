// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/endian.h"

namespace hg {

class FileHandle;

/// IO Handle endian wrapper.
class FileHandleEndian {
public:
	FileHandleEndian(std::shared_ptr<FileHandle> _h, Endianness _endianness = LittleEndian) : h(std::move(_h)), endianness(_endianness) {}

	operator bool() const { return bool(h); }

	inline FileHandle &GetIOHandle() const { return *h; }

	template <class T>
	const FileHandleEndian &operator<<(const T &v) {
		*h << ToHost(v, endianness);
		return *this;
	}

	template <class T>
	const FileHandleEndian &operator>>(T &v) {
		*h >> v;
		ToHostEndianness<sizeof(T)>(&v, endianness);
		return *this;
	}

private:
	std::shared_ptr<FileHandle> h;
	Endianness endianness;
};

} // namespace hg

