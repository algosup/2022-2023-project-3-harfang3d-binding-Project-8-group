// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/eye.h"
#include "foundation/frustum.h"
#include "foundation/matrix4.h"
#include "foundation/matrix44.h"
#include "foundation/rect.h"

namespace hg {

struct ViewState {
	fRect viewport, clipping;
	Matrix4 view, previous_view;
	Matrix44 projection;
	FrustumPlanes frustum_planes;
	Eye eye;
};

} // namespace hg
