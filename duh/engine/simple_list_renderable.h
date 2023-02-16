// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/renderable.h"
#include <memory>
#include <vector>

namespace hg {

/// Simple list renderable
class SimpleCullingSystem : public Renderable {
public:
	/// Add a renderable to the culling system.
	void AddRenderable(std::shared_ptr<Renderable> renderable);
	/// Delete a renderable from the culling system.
	void DeleteRenderable(const std::shared_ptr<Renderable> &renderable);

	/// Compute system world bounding box.
	MinMax GetWorldMinMax() const override;
	/// Cull system.
	Visibility Cull(const FrustumPlanes &frustum) override { return Inside; } // culling is done by GetRenderPrimitives
	/// Get renderable primitives.
	void GetRenderPrimitives(const FrustumPlanes &frustum, const Matrix4 &frustum_world_matrix, const FrustumPlanes &view_frustum, const Matrix4 &view_matrix, RenderPrimitiveLists &primitive_lists, RenderableContext ctx) override;

	const std::vector<std::shared_ptr<Renderable>> &GetRenderables() const { return renderables; }
	size_t GetRenderableCount() const { return renderables.size(); }

protected:
	std::vector<std::shared_ptr<Renderable>> renderables;
};

} // namespace hg
