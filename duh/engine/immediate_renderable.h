// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/gpu_types.h"
#include "engine/renderable.h"
#include "foundation/matrix4.h"
#include <memory>
#include <vector>

namespace hg {

class RenderMaterial;
class RenderGeometry;

struct DrawGeometryCommand {
	DrawGeometryCommand(std::shared_ptr<RenderGeometry> geo_, const Matrix4 &world_) : geo(geo_), world(world_) {}
	DrawGeometryCommand(DrawGeometryCommand &&c) : geo(std::move(c.geo)), world(std::move(c.world)) {}

	std::shared_ptr<RenderGeometry> geo;
	Matrix4 world;
};

struct GpuBuffer;
struct VertexLayout;
struct RenderPrimitive;

struct ImmediateRenderable : Renderable {
	void Reset();

	void AddDrawBuffersPrimitive(uint32_t idx_count, std::shared_ptr<GpuBuffer> idx, std::shared_ptr<GpuBuffer> vtx, const VertexLayout &vtx_layout, std::shared_ptr<RenderMaterial> mat, const MinMax &local_minmax, const Matrix4 &world, IndexType idx_type = IndexUShort, GpuPrimitiveType prim_type = GpuPrimitiveTriangle);
	void AddDrawGeometryCommand(std::shared_ptr<RenderGeometry> geo, const Matrix4 &world);

	MinMax GetWorldMinMax() const override { return minmax; }
	Visibility Cull(const FrustumPlanes &frustum) override;
	void GetRenderPrimitives(const FrustumPlanes &frustum, const Matrix4 &frustum_matrix, const FrustumPlanes &view_frustum, const Matrix4 &view_matrix, RenderPrimitiveLists &primitive_lists, RenderableContext ctx) override;

	size_t GetRenderPrimitiveCount() const { return render_primitives.size(); }
	size_t GetDrawGeometryCommandCount() const { return draw_geometry_commands.size(); }

private:
	MinMax minmax;

	std::vector<RenderPrimitive> render_primitives; // immediate render primitives
	std::vector<DrawGeometryCommand> draw_geometry_commands;

	void GrowMinMax(const MinMax &mm);
};

} // namespace hg
