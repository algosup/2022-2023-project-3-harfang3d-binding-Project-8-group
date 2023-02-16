// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/joint.h"
#include "foundation/vector3.h"

namespace hg {

/// 6-DOF joint component
class D6Joint : public std::enable_shared_from_this<D6Joint>, public Joint {
public:
	D6Joint();

	decl_component_defaults(D6Joint, "D6Joint", "Joint");

	comp_decl_member_get_sync_set_async_noimpl(D6Joint, uint8_t, AxisLock, axis_lock);

	comp_decl_member_get_sync_set_async_noimpl(D6Joint, JointLimit, XAxisLimit, x_axis_limit);
	comp_decl_member_get_sync_set_async_noimpl(D6Joint, JointLimit, YAxisLimit, y_axis_limit);
	comp_decl_member_get_sync_set_async_noimpl(D6Joint, JointLimit, ZAxisLimit, z_axis_limit);
	comp_decl_member_get_sync_set_async_noimpl(D6Joint, JointLimit, RotXAxisLimit, rot_x_axis_limit);
	comp_decl_member_get_sync_set_async_noimpl(D6Joint, JointLimit, RotYAxisLimit, rot_y_axis_limit);
	comp_decl_member_get_sync_set_async_noimpl(D6Joint, JointLimit, RotZAxisLimit, rot_z_axis_limit);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }
};

} // namespace hg
