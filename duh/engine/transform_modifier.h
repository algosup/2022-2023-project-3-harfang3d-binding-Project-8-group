// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"

namespace hg {

struct TransformModifier : Component {
	virtual void ModifyTransform(Matrix4 &world) = 0;
};

} // namespace hg
