// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/minmax.h"
#include <memory>
#include <vector>

namespace hg {

/// Result of tracing a ray through a bounding interval hierarchy
struct BIHTrace {
	explicit BIHTrace(bool closest = true) : want_closest(closest) {}

	bool want_closest{false};
	bool has_i{false}; ///< Do we have an intersection.

	float i_t{0}; ///< Distance to intersection from ray's origin.

	Vector3 s; ///< Ray origin.
	Vector3 d; ///< Ray direction.

	uint32_t node_visited{0}; ///< Number of nodes visited.
};

/**
	@short Bounding interval hierarchy

	A generic bounding interval hierarchy implementation.
	In order to use this structure on a custom set of data, you need to derive
	from this class and provide the implementation for the TraceLeaf method.

	In order to build the structure you simply provide a list of volumes to the
	build() methods.
*/
class BIHTree {
public:
	BIHTree();
	virtual ~BIHTree();

	/// Intersect tree with an axis aligned bounding box.
	uint32_t Intersect(MinMax &minmax, uint32_t *index_array, uint32_t max_index);
	/*
		@short Raytrace hierarchy.
		@note Ray direction must be normalized.
	*/
	virtual void Raytrace(BIHTrace &trace, const Vector3 &s, const Vector3 &d, float l = -1.f, void *parm = nullptr);

	/// Build hierarchy from a set of input volumes.
	virtual bool Build(uint32_t volume_count, MinMax *varray);

	/// Delete hierarchy.
	virtual void Free();

protected:
	struct Node {
		~Node();

		char axis{AxisLast}; ///< TODO Packed bitfield!

		union {
			uint32_t *idxs{nullptr}; ///< Index array
			Node *children; ///< Child nodes
		};

		union {
			uint32_t count{0}; ///< Index count in leaf.
			float split[2]; ///< Split planes.
		};
	};

	/// Minimum leaf volume count.
	uint32_t min_leaf_vcount;

	MinMax minmax; ///< Hierarchy bounding box.
	std::unique_ptr<Node> root; ///< Hierarchy root node.

	std::vector<uint32_t> sarray; ///< Index array.

	uint32_t node_count;
	uint32_t leaf_count;
	uint32_t depth;

	/**
		@name Trace functions
		@{
	*/
	/// Trace a leaf.
	virtual void TraceLeaf(Node *leaf, float tmin, float tmax, BIHTrace &trace, void *parm = nullptr) = 0;
	/// @}

	/**
		@name Intersection functions
		@{
	*/
	/// Intersect a node.
	uint32_t IntersectNode(Node *node, MinMax &sub_mm, uint32_t *index_array, uint32_t max_index);
	/// @}

	/**
		@name Build functions
		@{
	*/
	/// Create a leaf.
	void MakeNodeLeaf(Node *node, uint32_t count, uint32_t *sarray, MinMax *varray);
	/// Perform a node split.
	void DoNodeSplit(MinMax &minmax, uint32_t count, uint32_t *sarray, MinMax *varray, uint32_t &pivot, Node *node, uint32_t &split_axis);
	/// Split an index list of volumes.
	bool Split(MinMax &minmax, uint32_t count, uint32_t *sarray, MinMax *varray, Node *node, uint32_t depth);
	/// @}
};

} // namespace hg
