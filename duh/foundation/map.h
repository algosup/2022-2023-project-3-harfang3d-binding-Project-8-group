// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <map>

namespace hg {

template <typename K, typename V, typename M>
const V &MapGetWithDefault(const M &map, const K &key, const V &dflt) {
	auto i = map.find(key);
	return i != map.end() ? i->second : dflt;
}

} // namespace hg
