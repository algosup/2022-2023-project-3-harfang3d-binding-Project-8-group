// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "foundation/matrix4.h"

namespace hg {

/// Collision component
class Collision : public Component {
public:
	Collision();

	comp_decl_member_get_sync_set_async_noimpl(Collision, float, Mass, mass);
	comp_decl_member_get_sync_set_async_noimpl(Collision, Matrix4, Matrix, matrix);

	void *impl;

protected:
	void OnModified();
};

class Node;

/// Return true if the passed node has at least one collision component.
bool NodeHasCollisionAspect(const Node &node);
/// Return true if the component implements a collision aspect.
bool IsCollisionComponent(const Component &component);

} // namespace hg
