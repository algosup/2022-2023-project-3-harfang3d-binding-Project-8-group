// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "foundation/vector3.h"

#undef min
#undef max

namespace hg {

struct JointLimit {
	JointLimit(float mn = 0, float mx = 0) : min(mn), max(mx) {}
	bool operator==(const JointLimit &o) const { return min == o.min && max == o.max; }
	float min, max;
};

class Joint : public Component {
public:
	Joint();

	comp_decl_member_get_sync_set_async_noimpl(Joint, Vector3, Pivot, pivot);
	comp_decl_member_get_sync_set_async_noimpl(Joint, std::shared_ptr<Node>, OtherBody, other_body);
	comp_decl_member_get_sync_set_async_noimpl(Joint, Vector3, OtherPivot, other_pivot);

	void *impl{nullptr};

protected:
	void OnModified();
};

} // namespace hg
