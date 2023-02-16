// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/vector3.h"
#include "foundation/visibility.h"

namespace hg {

/// Helper function to test overlap on a given axis.
Visibility SATTestOverlap(const Vector3 &axis, int ca, const Vector3 *a, int cb, const Vector3 *b);

} // namespace hg
