// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/tangent_frame.h"
#include "engine/uv_set.h"
#include "foundation/color.h"
#include "foundation/matrix4.h"
#include "foundation/minmax.h"
#include "foundation/unit.h"
#include <array>
#include <memory>
#include <vector>

namespace hg {

struct GeometrySkin {
	bool operator==(const GeometrySkin &s) const {
		for (int i = 0; i < 4; ++i)
			if (w[i] != s.w[i] || bone_index[i] != s.bone_index[i])
				return false;
		return true;
	}

	uint8_t w[4];
	uint16_t bone_index[4];
};

struct Polygon {
	uint8_t vtx_count{0};
	uint8_t material{0};
	uint32_t *binding{nullptr};
};

struct VertexToPolygon {
	uint16_t pol_count;
	std::vector<uint32_t> pol_index; // polygon index in the geometry
	std::vector<uint8_t> vtx_index; // vertex index in the polygon
};

struct PolygonVertex {
	uint32_t pol_index;
	uint32_t vtx_index;
};

struct VertexToVertex {
	uint16_t vtx_count;
	std::vector<PolygonVertex> vtx;
};

typedef Polygon *pPolygon;

enum GeometryFlag : uint32_t {
	GeometryFlagDefault = 0x0,

	GeometryFlagHidden = 0x01,
	GeometryFlagNullLodProxy = 0x02,
	GeometryFlagNullShadowProxy = 0x04,

	// non-serialized
	GeometryFlagNoMaterialCache = 0x08
};

/// 3D model representation as vertex, polygon and materials
class Geometry {
public:
	static const int UVChannelCount = 4;

	std::string name;
	GeometryFlag flag{GeometryFlagDefault};

	std::string lod_proxy;
	float lod_distance{Mtr(100.f)};

	std::string shadow_proxy;

	/// Return the number of triangles in the mesh.
	size_t GetTriangleCount() const {
		size_t c = 0;
		for (auto &p : pol)
			if (p.vtx_count >= 3)
				c += p.vtx_count - 2;
		return c;
	}

	/**
		@short Flag vertices sharing the same properties as homogeneous.

		All vertices attributes are checked and compared. If they all lie
		under a given threshold then the vertex is reported as homogeneous.
		This function is heavily used by the geometry stripper and triangle
		list builder.

		@param material_index	Optionally restrict attribute comparison
								to one material only.
								(default: -1 to include all materials in
								the geometry.)
	*/
	void FlagHomogeneousVertex(std::vector<char> &flag, const std::vector<uint32_t> &polygon_index, const std::vector<VertexToPolygon> &, int material_index = -1) const;

	/**
		@name Topology
		@{
	*/
	/// Compute the local geometry bounding box.
	MinMax ComputeLocalMinMax() const;
	/// Compute geometry bones bounding volumes.
	bool ComputeLocalBoneMinMax(std::vector<MinMax> &bone_minmax) const;

	std::vector<Vector3> vtx, vtx_normal;
	std::vector<TangentFrame> vtx_tangent;
	std::vector<Color> rgb;
	std::array<UVSet, UVChannelCount> uv;

	std::vector<Polygon> pol;
	std::vector<uint32_t> binding;

	void AllocateVertex(uint32_t count);
	void AllocatePolygon(uint32_t count);

	/// Allocate polygon binding array.
	bool AllocatePolygonBinding();
	/// Compute polygon binding count.
	size_t ComputePolygonBindingCount() const;
	/// @}

	/**
		@name Modifier API
		@{
	*/
	void SetName(const std::string &name);

	void AllocateVertexNormal(uint32_t count);
	void AllocateVertexTangent(uint32_t count);
	void AllocateRgb(uint32_t count);
	void AllocateMaterialTable(uint32_t count);
	bool AllocateUVChannels(uint8_t channel_count, uint32_t uv_per_channel);

	size_t GetVertexCount() const { return vtx.size(); }
	size_t GetPolygonCount() const { return pol.size(); }
	size_t GetVertexNormalCount() const { return vtx_normal.size(); }
	size_t GetVertexTangentCount() const { return vtx_tangent.size(); }
	size_t GetRgbCount() const { return rgb.size(); }
	int GetUVCount() const;

	bool SetVertex(uint32_t index, const Vector3 &vertex);
	Vector3 GetVertex(uint32_t index) const;
	bool SetVertexNormal(uint32_t index, const Vector3 &vertex);
	Vector3 GetVertexNormal(uint32_t index) const;
	bool SetRgb(uint32_t poly_index, uint32_t count, const Color *colors);
	bool SetRgb(uint32_t poly_index, const std::vector<Color> &colors);
	Color GetRgb(uint32_t index) const;
	bool SetUV(uint32_t channel, uint32_t poly_index, uint32_t count, const Vector2 *uvs);
	bool SetUV(uint32_t channel, uint32_t poly_index, const std::vector<Vector2> &uvs);
	Vector2 GetUV(uint32_t channel, uint32_t index) const;

	bool SetPolygonVertexCount(uint32_t index, uint8_t vtx_count);
	bool SetPolygonMaterialIndex(uint32_t index, uint8_t material);
	bool SetPolygon(uint32_t index, uint8_t vtx_count, uint8_t material);
	int GetPolygonVertexCount(uint32_t index) const;
	int GetPolygonMaterialIndex(uint32_t index) const;
	std::vector<uint32_t> GetPolygonBinding(uint32_t index) const;

	bool SetPolygonBinding(uint32_t index, uint32_t count, const int *idx);
	bool SetPolygonBinding(uint32_t index, std::vector<int> &idx);

	float ComputePolygonArea(uint32_t index) const;

	bool Validate() const;
	/// @}

	/**
		@name Skinning
		@{
	*/
	/// Allocate bones array (not the skin weights array).
	void AllocateBone(uint32_t bone_count);
	/// Return the number of bones referenced by the geometry.
	size_t GetBoneCount() const;

	std::vector<std::string> bone_name;
	std::vector<Matrix4> bone_bind_matrix;

	std::vector<GeometrySkin> skin; ///< skin weights
	/// @}

	std::vector<Vector3> pol_normal;
	std::vector<TangentFrame> pol_tangent;

	bool ComputePolygonNormal(bool force = false);
	bool ComputePolygonTangent(uint32_t uv_index = 0, bool force = false);

	bool ComputeVertexNormal(float msa = 0.7f, bool force = false);
	bool ComputeVertexNormal(std::vector<Vector3> &normal_out, float msa = 0.7f);
	bool ComputeVertexTangent(bool reverse_T = false, bool reverse_B = false, bool force = false);

	void ReverseTangentFrame(bool reverse_T, bool reverse_B);

	/**
		@name Geometry tools
		@{
	*/
	/// Compute polygon start index.
	std::vector<uint32_t> ComputePolygonIndex() const;
	/// Compute vertex to polygon table.
	std::vector<VertexToPolygon> ComputeVertexToPolygon() const;
	/// Compute vertex to vertex table.
	std::vector<VertexToVertex> ComputeVertexToVertex(const std::vector<VertexToPolygon> &vtx_to_poly) const;
	/// @}

	/**
		@name Material
		@{
	*/
	std::vector<std::string> materials;

	/// Merge duplicate materials in table.
	size_t MergeDuplicateMaterials();
	/// @}
};

/// Compute the minmax of a vertex array.
MinMax ComputeVertexArrayMinMax(const std::vector<Vector3> &vtx);
MinMax ComputeVertexArrayMinMax(const std::vector<Vector3> &vtx, const Matrix4 &transform);

void GeometrySmoothRGB(Geometry &geo, int pass_count, float max_smooth_angle);

} // namespace hg
