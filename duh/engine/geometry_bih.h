// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/geometry.h"
#include "engine/geometry_tree.h"
#include "engine/material.h"
#include "foundation/bih.h"

namespace hg {

/// BIH ray/triangle intersection acceleration structure.
struct GeometryBIHAccel {
	char cu, cv;
	std::vector<float> k;
	float d;
};

struct GeometryBIHMaterial {
	std::shared_ptr<Material> material;
};

/*
	@short Geometry polygon bounding interval hierarchy

	Raytracing/intersection acceleration structure theoretically
	performing within 70% of the SAH/KD-Tree performances.
*/
class GeometryBIH : public GeometryTree, public BIHTree {
public:
	/// Return the raytracing LUT structure.
	const std::vector<GeometryBIHAccel> &GetRaytracingAccelerationStructure() const { return acc; }

	/// Fast polygon test.
	bool FastPolyTest(uint32_t ip, Vector3 &s, Vector3 &d, float l = -1.f);

	void RaytraceGeometry(GeometryTrace &trace, const Vector3 &s, const Vector3 &d, float l = -1.f) override;
	bool BuildFromGeometry(std::shared_ptr<Geometry> geo) override;
	void Free() override;

private:
	std::vector<GeometryBIHMaterial> material_table; ///< material table
	std::vector<uint32_t> pol_index;

	std::vector<GeometryBIHAccel> acc; ///< raytracing lookup tables

	/// Trace leaf content.
	void TraceLeaf(Node *leaf, float tmin, float tmax, BIHTrace &trace, void *parm = nullptr) override;
};

/// Geometry BIH tree
class GeometryBIHTree : public GeometryTree {
public:
	~GeometryBIHTree() { Free(); }

	/**
		@name Core functions.
		@{
	*/
	/// Raytrace the geometry tree.
	void RaytraceGeometry(GeometryTrace &trace, const Vector3 &s, const Vector3 &d, float l = -1.f) override { bih.RaytraceGeometry(trace, s, d, l); }
	/// Build tree from a geometry.
	bool BuildFromGeometry(std::shared_ptr<Geometry> geo) override { return bih.BuildFromGeometry(geo); }
	/// Free all internal structures.
	void Free() override { bih.Free(); }
	/// @}

private:
	GeometryBIH bih;
};

} // namespace hg
