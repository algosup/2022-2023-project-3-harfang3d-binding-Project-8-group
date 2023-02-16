// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstddef>
#include <vector>

namespace hg {

template <typename T>
size_t memory_usage(const std::vector<T> &v) { return v.size() * sizeof(T); }

} // namespace hg
