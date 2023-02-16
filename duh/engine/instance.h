// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "engine/group.h"
#include "foundation/string.h"

namespace hg {

/// Scene instance.
class Instance : public std::enable_shared_from_this<Instance>, public Component {
public:
	decl_component_defaults(Instance, "Instance", "Instance");

	Instance();

	comp_decl_member_get_sync_set_async_noimpl(Instance, std::string, Path, path);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	ComponentState GetState() const override;

	std::shared_ptr<Group> group;

	size_t instance_depth{0};

private:
	friend void InstanceSetInternalComponentState(Instance &instance, ComponentState state);

	ComponentState state;
};

} // namespace hg
