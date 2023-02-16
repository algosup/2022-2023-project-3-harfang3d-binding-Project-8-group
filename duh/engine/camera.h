// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "foundation/vector2.h"

namespace hg {

class Matrix4;
class Matrix44;

/// Component to use a Node as a Scene viewpoint
class Camera : public std::enable_shared_from_this<Camera>, public Component {
public:
	Camera();

	decl_component_defaults(Camera, "Camera", "Camera");

	comp_decl_member_get_sync_set_async_validated(Camera, float, ZoomFactor, zoom_factor, ClampToZero);
	comp_decl_member_get_sync_set_async_validated(Camera, float, ZNear, z_near, ZNearValidator);
	comp_decl_member_get_sync_set_async_validated(Camera, float, ZFar, z_far, ZFarValidator);

	comp_decl_member_get_sync_set_async(Camera, bool, Orthographic, is_orthographic);
	comp_decl_member_get_sync_set_async_validated(Camera, float, OrthographicSize, orthographic_size, ClampToZero);

	/// Compute and return this component projection matrix.
	Matrix44 GetProjectionMatrix(const Vector2 &aspect_ratio) const;

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

private:
	static float ZNearValidator(Camera *c, float v) { return Min(v, c->GetZFar()); }
	static float ZFarValidator(Camera *c, float v) { return Max(v, c->GetZNear()); }
};

/// Project a vector using a node camera aspect.
bool Project(const Matrix4 &camera_world, float zoom_factor, const Vector2 &ar, const Vector3 &in, Vector3 &out);
/// Unproject a vector from to 2d to 3d using a node camera aspect.
bool Unproject(const Matrix4 &camera_world, float zoom_factor, const Vector2 &ar, const Vector3 &in, Vector3 &out);

} // namespace hg
