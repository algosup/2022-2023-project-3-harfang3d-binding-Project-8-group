// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/display_list.h"
#include "engine/skeleton.h"

namespace hg {

struct RenderPrimitiveLists;

struct RenderPrimitive {
	RenderPrimitive(uint32_t idx_count_, IndexType idx_type_, GpuPrimitiveType prim_type_, std::shared_ptr<GpuBuffer> idx_, std::shared_ptr<GpuBuffer> vtx_,
		const VertexLayout &vtx_layout_, const Matrix4 &previous_world_, const Matrix4 &world_, const MinMax &world_minmax_, std::shared_ptr<Skeleton> skeleton_, std::shared_ptr<RenderMaterial> material_,
		uint32_t picking_id_)
		: idx_count(idx_count_), idx_type(idx_type_), prim_type(prim_type_), idx(std::move(idx_)), vtx(std::move(vtx_)), vtx_layout(vtx_layout_), previous_world(previous_world_),
		  world(world_), world_minmax(world_minmax_), skeleton(skeleton_), material(std::move(material_)), picking_id(picking_id_) {}

	RenderPrimitive(uint32_t idx_count_, IndexType idx_type_, GpuPrimitiveType prim_type_, std::shared_ptr<GpuBuffer> idx_, std::shared_ptr<GpuBuffer> vtx_, const VertexLayout &vtx_layout_,
		const Matrix4 &previous_world_, const Matrix4 &world_, const MinMax &world_minmax_, std::shared_ptr<RenderMaterial> material_, uint32_t picking_id_)
		: idx_count(idx_count_), idx_type(idx_type_), prim_type(prim_type_), idx(std::move(idx_)), vtx(std::move(vtx_)), vtx_layout(vtx_layout_), previous_world(previous_world_), 
		  world(world_), world_minmax(world_minmax_), material(std::move(material_)), picking_id(picking_id_) {}

	uint32_t idx_count;
	IndexType idx_type;

	GpuPrimitiveType prim_type;

	std::shared_ptr<GpuBuffer> idx;
	std::shared_ptr<GpuBuffer> vtx;
	VertexLayout vtx_layout;

	Matrix4 previous_world;
	Matrix4 world;
	MinMax world_minmax;

	std::shared_ptr<Skeleton> skeleton;

	std::shared_ptr<RenderMaterial> material;
	uint32_t picking_id;
};

/// Draw multiple render primitives.
void DrawRenderPrimitives(RenderSystem &render_system, const RenderPrimitive *prims, uint32_t count, const RenderPass &pass);

//
class RenderStateCache;

struct CustomRenderPrimitive {
	virtual ~CustomRenderPrimitive() {}

	/// Draw custom primitive.
	virtual void Draw(RenderSystem &render_system, RenderStateCache &state_cache, const RenderPass &pass) = 0;
	/// Return the primitive world minmax.
	virtual MinMax GetWorldMinMax() const = 0;
};

//
struct RenderPrimitiveLists {
	/// Return the number of primitive for a given pass.
	inline size_t Size(RenderPass::Pass pass) const { return prims[pass].size() + customs[pass].size(); }
	/// Clear all primitive lists.
	void Clear();

	std::vector<RenderPrimitive> prims[RenderPass::PassCount];
	std::vector<std::shared_ptr<CustomRenderPrimitive>> customs[RenderPass::PassCount];
};

void DrawRenderPrimitives(RenderSystem &render_system, const RenderPrimitiveLists &prim_lists, const RenderPass &pass);

} // namespace hg
