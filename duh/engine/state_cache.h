// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/render_pass.h"
#include "engine/vertex_layout.h"
#include "foundation/cext.h"
#include "foundation/matrix4.h"

namespace hg {

struct Skeleton;
struct GpuShader;
struct GpuBuffer;
class RenderMaterial;
class RenderSystem;

/// Cache to limit render system and renderer state changes.
class RenderStateCache {
public:
	struct Stats {
		uint32_t previous_world_change;
		uint32_t world_change;
		uint32_t picking_id_change;
		uint32_t skeleton_change;
		uint32_t shader_change;
		uint32_t material_change;
		uint32_t index_buffer_change;
		uint32_t vertex_buffer_change;
		uint32_t vertex_layout_change;
		uint32_t surface_draw_state_change;
		uint32_t pass_change;
		uint32_t light_change;
		uint32_t commit_count;
	};

	RenderStateCache();
	~RenderStateCache();

	void Reset();

#define SetEntry(C, O, V) C = set_if_not_equal(O, V);

	inline void SetPreviousWorld(const Matrix4 &w) { SetEntry(previous_world_change, previous_world, w); }
	inline void SetWorld(const Matrix4 &w) { SetEntry(world_change, world, w); }
	inline void SetPickingID(uint32_t id) { SetEntry(picking_id_change, picking_id, id); }
	inline void SetSkeleton(const std::shared_ptr<Skeleton> &s) { SetEntry(skeleton_change, skeleton, s); }
	inline void SetShader(const std::shared_ptr<GpuShader> &s) { SetEntry(shader_change, shader, s); }
	inline void SetMaterial(const std::shared_ptr<RenderMaterial> &m) { SetEntry(material_change, material, m); }
	inline void SetIndexBuffer(const std::shared_ptr<GpuBuffer> &b) { SetEntry(index_buffer_change, idx, b); }
	inline void SetVertexBuffer(const std::shared_ptr<GpuBuffer> &b) { SetEntry(vertex_buffer_change, vtx, b); }
	inline void SetVertexLayout(const VertexLayout &l) { SetEntry(vertex_layout_change, vtx_layout, l); }
	inline void SetSurfaceDrawState(uint32_t state) { SetEntry(surface_draw_state_change, surface_draw_state, state); }
	inline void SetRenderPass(RenderPass::Pass p) { SetEntry(pass_change, pass, p); }
	inline void SetLight(const std::shared_ptr<Node> &l) { SetEntry(light_change, light, l); }

	bool Commit(RenderSystem &render_system);

	void ResetStat();
	const Stats &GetStat() const { return stat; }

private:
	Stats stat;

	uint32_t previous_world_change : 1;
	uint32_t world_change : 1;
	uint32_t picking_id_change : 1;
	uint32_t skeleton_change : 1;
	uint32_t shader_change : 1;
	uint32_t material_change : 1;
	uint32_t index_buffer_change : 1;
	uint32_t vertex_buffer_change : 1;
	uint32_t vertex_layout_change : 1;
	uint32_t surface_draw_state_change : 1;
	uint32_t pass_change : 1;
	uint32_t light_change : 1;

	Matrix4 previous_world;
	Matrix4 world;
	uint32_t picking_id;
	std::shared_ptr<Skeleton> skeleton;
	std::shared_ptr<GpuShader> shader;
	uint32_t surface_draw_state;
	std::shared_ptr<RenderMaterial> material;
	std::shared_ptr<GpuBuffer> idx;
	std::shared_ptr<GpuBuffer> vtx;
	VertexLayout vtx_layout;
	RenderPass::Pass pass;
	std::shared_ptr<Node> light;
};

struct RasterFont;

} // namespace hg
