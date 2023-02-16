// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/collision.h"

namespace hg {

/// Sphere collision component
class SphereCollision : public std::enable_shared_from_this<SphereCollision>, public Collision {
public:
	SphereCollision();

	decl_component_defaults(SphereCollision, "SphereCollision", "Collision");

	comp_decl_member_get_sync_set_async_noimpl(SphereCollision, float, Radius, radius);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }
};

} // namespace hg
