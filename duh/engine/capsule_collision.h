// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/collision.h"

namespace hg {

/// Capsule collision component
class CapsuleCollision : public std::enable_shared_from_this<CapsuleCollision>, public Collision {
public:
	CapsuleCollision();

	decl_component_defaults(CapsuleCollision, "CapsuleCollision", "Collision");

	comp_decl_member_get_sync_set_async_noimpl(CapsuleCollision, float, Length, length);
	comp_decl_member_get_sync_set_async_noimpl(CapsuleCollision, float, Radius, radius);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }
};

} // namespace hg
