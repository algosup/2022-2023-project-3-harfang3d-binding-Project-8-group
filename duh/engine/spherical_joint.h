// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/joint.h"

namespace hg {

/// Spherical joint component
class SphericalJoint : public std::enable_shared_from_this<SphericalJoint>, public Joint {
public:
	SphericalJoint();

	decl_component_defaults(SphericalJoint, "SphericalJoint", "Joint");

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }
};

} // namespace hg
