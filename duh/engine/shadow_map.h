// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <vector>
#include <memory>

namespace hg {

class task_set;

class Node;
class RenderSystem;

struct Renderable;
struct RenderTarget;

void PrepareShadowMap(const RenderSystem &render_system, const std::vector<std::shared_ptr<Renderable>> &renderables, const Node &light, bool rebuild_frustum_primitive_lists, const std::shared_ptr<task_set> &set);
void DrawShadowMap(RenderSystem &render_system, const Node &light, std::shared_ptr<RenderTarget> shadow_map_fbo);

} // namespace hg
