// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/collision.h"

namespace hg {

class Geometry;

/// Convex collision component
class ConvexCollision : public std::enable_shared_from_this<ConvexCollision>, public Collision {
public:
	ConvexCollision();

	decl_component_defaults(ConvexCollision, "ConvexCollision", "Collision");

	comp_decl_member_get_sync_set_async_noimpl(ConvexCollision, std::shared_ptr<Geometry>, Geometry, geometry);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }
};

} // namespace hg
