// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <string>

namespace hg {

template <typename T>
const std::string &GetTypeName() {
	static_assert(!sizeof(T), "Missing GetTypeName specialization");
	static std::string dummy;
	return dummy;
}

} // namespace hg
