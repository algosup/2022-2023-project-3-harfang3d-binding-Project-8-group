// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/vector3_api.h"

namespace hg {

struct TangentFrame {
	Vector3 T, B;
};

inline bool operator==(const TangentFrame &a, const TangentFrame &b) { return a.T == b.T && a.B == b.B; }
inline bool operator!=(const TangentFrame &a, const TangentFrame &b) { return a.T != b.T || a.B != b.B; }

} // namespace hg
