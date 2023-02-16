// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "engine/gpu_resource.h"
#include "foundation/color.h"

namespace hg {

/// Describes Scene properties such as background color, ambient color or fog configuration
class Environment : public std::enable_shared_from_this<Environment>, public Component {
public:
	decl_component_defaults(Environment, "Environment", "Environment");

	Environment();

	comp_decl_member_get_sync_set_async_validated(Environment, float, TimeOfDay, time_of_day, ClampToZeroOne);

	comp_decl_member_get_sync_set_async(Environment, bool, ClearBackgroundColor, clear_bg_color);
	comp_decl_member_get_sync_set_async(Environment, bool, ClearBackgroundDepth, clear_bg_depth);
	comp_decl_member_get_sync_set_async(Environment, Color, BackgroundColor, background_color);

	comp_decl_member_get_sync_set_async_validated(Environment, float, AmbientIntensity, ambient_intensity, ClampToZero);
	comp_decl_member_get_sync_set_async(Environment, Color, AmbientColor, ambient_color);

	comp_decl_member_get_sync_set_async(Environment, Color, FogColor, fog_color);
	comp_decl_member_get_sync_set_async_validated(Environment, float, FogNear, fog_near, ClampToZero);
	comp_decl_member_get_sync_set_async_validated(Environment, float, FogFar, fog_far, ClampToZero);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }
};

} // namespace hg
