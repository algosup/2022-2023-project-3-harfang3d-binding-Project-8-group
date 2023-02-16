// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/minmax.h"
#include "foundation/visibility.h"
#include "engine/renderable_context.h"

namespace hg {

class Matrix4;

struct FrustumPlanes;
struct RenderPrimitiveLists;

/// Renderable interface
struct Renderable {
	virtual ~Renderable() {}

	/// Get renderable world minmax.
	virtual MinMax GetWorldMinMax() const = 0;

	/// Cull renderable.
	virtual Visibility Cull(const FrustumPlanes &frustum) = 0;
	/// Get renderable primitives.
	virtual void GetRenderPrimitives(const FrustumPlanes &frustum, const Matrix4 &frustum_matrix, const FrustumPlanes &view_frustum, const Matrix4 &view_matrix, RenderPrimitiveLists &primitive_lists, RenderableContext ctx) = 0;
};

} // namespace hg
