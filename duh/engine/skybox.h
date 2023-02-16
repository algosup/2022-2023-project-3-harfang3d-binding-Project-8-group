// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"

namespace hg {

struct GpuShader;
struct Texture;

/// Skybox component
class Skybox : public std::enable_shared_from_this<Skybox>, public Component {
	decl_component_defaults(Skybox, "Skybox", "Skybox");

	Skybox();

	comp_decl_member_get_sync_set_async(Skybox, std::shared_ptr<GpuShader>, Shader, shader);
	comp_decl_member_get_sync_set_async(Skybox, std::shared_ptr<Texture>, LayerA, layer_a);
	comp_decl_member_get_sync_set_async(Skybox, std::shared_ptr<Texture>, LayerB, layer_b);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }
};

} // namespace hg
