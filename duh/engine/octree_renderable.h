// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/renderable.h"
#include <memory>
#include <vector>

namespace hg {

class RenderSystem;

/// Octree renderable
class OctreeRenderable : public Renderable {
public:
	void AddRenderable(std::shared_ptr<Renderable> renderable);
	void DeleteRenderable(const std::shared_ptr<Renderable> &renderable);

	bool Update();

	/// Compute renderable world bounding box.
	MinMax GetWorldMinMax() const override;
	/// Cull system.
	Visibility Cull(const FrustumPlanes &frustum) override { return Inside; }
	/// Fill render primitive lists.
	void GetRenderPrimitives(const FrustumPlanes &frustum, const Matrix4 &frustum_matrix, const FrustumPlanes &view_frustum, const Matrix4 &view_matrix, RenderPrimitiveLists &primitive_lists, RenderableContext ctx) override;

	/// Set the maximum distance.
	void SetMaxDistance(float d = 0.f) { max_dist = d; }

	size_t GetRenderableCount() const { return renderables.size(); }

	void DrawDebugVisuals(RenderSystem &render_system);

private:
	struct CachedNode {
		std::shared_ptr<Renderable> renderable;
		MinMax minmax;
	};

	struct OctreeNode {
		MinMax minmax;
		std::vector<CachedNode *> cached_node;
		std::unique_ptr<OctreeNode> child[2];
	};

	bool dirty{true};
	float max_dist{0.f};

	std::vector<CachedNode> cached_nodes;
	std::unique_ptr<OctreeNode> root;

	std::vector<std::shared_ptr<Renderable>> renderables;

	OctreeNode *InsertList(std::vector<CachedNode *> &list);

	void GetNodeRenderPrimitives(OctreeNode *node, const FrustumPlanes &frustum, const Matrix4 &frustum_matrix, const FrustumPlanes &view_frustum, const Matrix4 &view_matrix, RenderPrimitiveLists &primitive_lists, RenderableContext ctx);
	void CullNodeRenderPrimitives(OctreeNode *node, const FrustumPlanes &frustum, const Matrix4 &frustum_matrix, const FrustumPlanes &view_frustum, const Matrix4 &view_matrix, RenderPrimitiveLists &primitive_lists, RenderableContext ctx);

	void DrawNodeDebugVisuals(RenderSystem &render_system, const OctreeNode *octree_node, uint32_t depth, std::vector<MinMax> &node_minmax, std::vector<MinMax> &leaf_minmax);
};

} // namespace hg
