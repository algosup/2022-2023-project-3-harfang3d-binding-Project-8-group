// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/collision.h"
#include "foundation/vector3.h"

namespace hg {

/// Box collision component
class BoxCollision : public std::enable_shared_from_this<BoxCollision>, public Collision {
public:
	BoxCollision();

	decl_component_defaults(BoxCollision, "BoxCollision", "Collision");

	comp_decl_member_get_sync_set_async_noimpl(BoxCollision, Vector3, Dimensions, dimensions);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }
};

} // namespace hg
