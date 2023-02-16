// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/geometry.h"
#include <vector>

namespace hg {

/**
	@short Geometry template class.
	@author Thomas Simonnet (scorpheus)
	@author Emmanuel Julien (ejulien@owloh.com)
*/
class GeometryTemplate {
public:
	static const int max_uv_in_template = 4;

	void Clear();
	void BeginPolygon();

	void PushVertex(const Vector3 &vertex);
	void PushNormal(const Vector3 &normal);
	void PushUV(uint32_t channel, const Vector2 &uv);
	void PushColor(const Color &color);

	void EndPolygon(uint8_t material_index);

	void ClearMaterials();
	void PushMaterial(const std::string &path);

	size_t GetMaterialCount() const;
	void SetMaterial(size_t idx, const std::string &path);

	/// Set the vertex merge threshold.
	void SetVertexMergeThreshold(float threshold) { merge_threshold = threshold; }

	/// Instantiate the current template description.
	std::shared_ptr<Geometry> Instantiate(const std::string &name);

private:
	struct Polygon {
		Polygon() = default;
		Polygon(Polygon &&p) : vertex(std::move(p.vertex)), normal(std::move(p.normal)), color(std::move(p.color)), material(p.material) {
			for (int i = 0; i < max_uv_in_template; ++i)
				uv[i] = std::move(p.uv[i]);
		}

		void Clear() {
			vertex.clear();
			normal.clear();
			color.clear();
			for (int i = 0; i < max_uv_in_template; ++i)
				uv[i].clear();
			material = 0;
		}

		std::vector<Vector3> vertex;
		std::vector<Vector3> normal;
		std::vector<Color> color;
		std::array<std::vector<Vector2>, max_uv_in_template> uv;
		uint8_t material{0};
	};

	struct PolygonVertex {
		uint32_t ipoly;
		uint32_t ivertex;
	};

	Vector3 GetVertex(const PolygonVertex &vtx);
	Vector3 GetNormal(const PolygonVertex &vtx);
	Vector2 GetUV(uint32_t channel, const PolygonVertex &vtx);

	float merge_threshold{0.0001f};

	Polygon polygon;
	std::vector<Polygon> polygons;
	std::vector<std::string> materials;
};

} // namespace hg
