// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "engine/recast_wrapper.h"

namespace hg {

class NavigationGeometry;

/// Navigation
class Navigation : public std::enable_shared_from_this<Navigation>, public Component {
public:
	decl_component_defaults(Navigation, "Navigation", "Navigation");

	Navigation();

	comp_decl_member_get_sync_set_async(Navigation, std::shared_ptr<NavigationGeometry>, Geometry, geometry);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	/// Find a path between two local positions on the navigation geometry.
	bool FindPathTo(const Vector3 &from, const Vector3 &to, NavigationPath &path, uint32_t layer_index = 0);
};

} // namespace hg
