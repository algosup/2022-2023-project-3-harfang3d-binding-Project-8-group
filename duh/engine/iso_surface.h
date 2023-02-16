// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/vector3.h"
#include <future>
#include <memory>
#include <vector>

namespace hg {

class RenderSystem;
class RenderMaterial;
class RenderGeometry;
class Geometry;

struct IsoSurface {
	static const int vertex_map_count = 256;

	IsoSurface();

	std::vector<uint32_t> indices;
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;

	void Clear();
	void AddTriangle(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2);

	inline size_t GetTriangleCount() const { return indices.size() / 3; }

private:
	uint32_t AddVertex(const Vector3 &v);

	std::vector<uint32_t> vertex_map[vertex_map_count];
};

void PolygoniseIsoSurface(uint32_t width, uint32_t height, uint32_t depth, const float *val, float isolevel, IsoSurface &out, const Vector3 &unit = Vector3::One);
void IsoSurfaceToCoreGeometry(const IsoSurface &iso, Geometry &out);

std::future<void> IsoSurfaceToRenderGeometry(std::shared_ptr<RenderSystem> render_system, std::shared_ptr<IsoSurface> iso, std::shared_ptr<RenderGeometry> geo, std::shared_ptr<RenderMaterial> mat);
std::future<void> PolygoniseIsoSurfaceToRenderGeometry(std::shared_ptr<RenderSystem> render_system, std::shared_ptr<RenderGeometry> geo, std::shared_ptr<RenderMaterial> mat, uint32_t width, uint32_t height, uint32_t depth, const float *val, float isolevel, std::shared_ptr<IsoSurface> iso, const Vector3 &unit = Vector3::One);

} // namespace hg
