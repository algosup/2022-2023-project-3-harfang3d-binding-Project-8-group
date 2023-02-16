// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/display_list.h"
#include "engine/renderable.h"
#include "engine/skeleton.h"

namespace hg {

class Geometry;
enum GeometryFlag : uint32_t;

class RenderGeometry : public GpuResource {
public:
	explicit RenderGeometry(const std::string &name = std::string());

	/// Change a material in the material table.
	bool SetMaterial(uint32_t index, std::shared_ptr<RenderMaterial> mat);
	/// Re-create from core geometry.
	void Create(RenderSystem &render_system, const std::shared_ptr<Geometry> &geo);
	/// Free geometry.
	void Free();

	bool IsReadyOrFailed() const override;

	std::vector<std::shared_ptr<RenderMaterial>> materials;

	GeometryFlag flag;

	MinMax minmax; // local minmax

	std::shared_ptr<RenderGeometry> lod_proxy;
	float lod_distance{0};

	std::shared_ptr<RenderGeometry> shadow_proxy;

	std::vector<Matrix4> bone_bind_matrix;
	std::vector<MinMax> bone_minmax;
	std::vector<DisplayList> display_list;
};

bool SaveRenderGeometry(const RenderGeometry &render_geo, Geometry &geo, const std::string &path);

/// Return the geometry level of detail for a given render context.
std::shared_ptr<RenderGeometry> GetGeometryLod(std::shared_ptr<RenderGeometry> geo, const Matrix4 &geo_world, const Matrix4 &view_world, RenderableContext ctx);

/// Cull and add all of a geometry render primitives matching the current draw configuration.
void SubmitGeometryRenderPrimitives(const RenderGeometry &geo, const Matrix4 &world, const Matrix4 &previous_world, const Matrix4 &view_world, uint32_t picking_id, RenderPrimitiveLists &prim_lists, RenderableContext ctx);
void SubmitGeometryRenderPrimitives(const RenderGeometry &geo, const Matrix4 &world, const Matrix4 &previous_world, const Matrix4 &view_world, uint32_t picking_id, const std::shared_ptr<Skeleton> &skeleton, RenderPrimitiveLists &prim_lists, RenderableContext ctx);

/// Create render geometry from core geometry.
void CoreGeometryToRenderGeometry(RenderSystem &render_system, const Geometry &geo, RenderGeometry &out);

} // namespace hg
