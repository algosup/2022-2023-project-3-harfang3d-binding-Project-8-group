// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/render_pass.h"
#include "engine/render_primitive.h"
#include "engine/renderable.h"

namespace hg {

class LightSystem;
struct RenderTarget;

void SetSurfaceDrawState(Renderer &renderer, uint32_t surface_draw_state, RenderPass::Pass pass, bool is_light_pass);

void DrawPrimitivesForward(RenderSystem &render_system, const std::vector<std::shared_ptr<Renderable>> &renderables, LightSystem &light_system, const RenderPrimitiveLists &prim_lists, RenderPass::Pass pass);
void DrawFramebufferDependentPrimitivesForward(RenderSystem &render_system, const std::vector<std::shared_ptr<Renderable>> &renderables, LightSystem &light_system, const RenderPrimitiveLists &prim_lists);

void DrawPrimitiveListsDeferred(RenderSystem &render_system, const std::vector<std::shared_ptr<Renderable>> &renderables, LightSystem &light_system, const RenderPrimitiveLists &prims);
void DrawPrimitiveListsForward(RenderSystem &render_system, const std::vector<std::shared_ptr<Renderable>> &renderables, LightSystem &light_system, const RenderPrimitiveLists &prims);

bool DrawRenderablesPicking(RenderSystem &render_system, const std::vector<std::shared_ptr<Renderable>> &renderables, const std::shared_ptr<RenderTarget> &render_target);

} // namespace hg
