// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/matrix3.h"
#include "foundation/matrix4.h"
#include "foundation/matrix44.h"
#include "foundation/rect.h"
#include "foundation/string.h"
#include "foundation/vector2.h"
#include "foundation/vector3.h"
#include "foundation/vector4.h"

namespace std {

string to_string(const hg::Vector2 &vec);
string to_string(const hg::Vector3 &vec);
string to_string(const hg::Vector4 &vec);
string to_string(const hg::Matrix3 &mtx);
string to_string(const hg::Matrix4 &mtx);
string to_string(const hg::Matrix44 &mtx);
string to_string(const hg::iRect &rect);
string to_string(const hg::fRect &rect);

} // namespace std
