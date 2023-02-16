// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/texture.h"
#include "foundation/get_type_name.h"
#include <array>
#include <memory>

namespace hg {

/// GPU render target
struct RenderTarget {
	uint32_t color_texture_count{0};
	std::array<std::shared_ptr<Texture>, 8> color_texture;
	std::shared_ptr<Texture> depth_texture;
};

void RenderTargetDeleter(RenderTarget *rt, std::weak_ptr<Renderer> renderer_weak, task_affinity worker);

} // namespace hg
