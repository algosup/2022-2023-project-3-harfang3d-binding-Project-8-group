// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/matrix4.h"
#include "foundation/matrix44.h"
#include "foundation/plane.h"
#include "foundation/vector2.h"
#include "foundation/visibility.h"
#include <array>

namespace hg {

/// Describes a perspective or orthographic view volume
struct Frustum {
	enum Type {
		Perspective,
		Orthographic
	};

	Type type{Perspective};

	float fov_size{0};
	Vector2 aspect_ratio{0, 0};
	float znear{0}, zfar{0};

	Matrix4 world;
};

Frustum MakePerspectiveFrustum(float znear, float zfar, float fov, const Vector2 &ar, const Matrix4 &m);
Frustum MakeOrthographicFrustum(float znear, float zfar, float size, const Vector2 &ar, const Matrix4 &m);

//
enum FrustumPlane {
	FrustumPlaneTop,
	FrustumPlaneBottom,
	FrustumPlaneLeft,
	FrustumPlaneRight,
	FrustumPlaneNear,
	FrustumPlaneFar,
	FrustumPlaneCount
};

/// Describes the planes forming a Frustum volume
struct FrustumPlanes {
	Plane plane[FrustumPlaneCount];
};

FrustumPlanes BuildFrustumPlanes(const Frustum &frustum);
std::array<Vector3, 8> BuildFrustumVertices(const Frustum &frustum);

FrustumPlanes BuildFrustumPlanesFromProjectionMatrix(const Matrix44 &projection);

struct MinMax;

/// Return the visibility flag of a vector set against this frustum.
Visibility ClassifyPoint(const FrustumPlanes &frustum, uint32_t count, const Vector3 *points, float distance = 0.f);
/// Return the visibility flag of a sphere against this frustum.
Visibility ClassifySphere(const FrustumPlanes &frustum, const Vector3 &origin, float radius);
/// Return the visibility flag of a minmax against this frustum.
Visibility ClassifyMinMax(const FrustumPlanes &frustum, const MinMax &minmax);

} // namespace hg
