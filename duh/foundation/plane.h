// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/matrix4.h"
#include "foundation/vector3.h"

namespace hg {

/**
	@short Plane
	ax + by + cz + d = 0
*/
class Plane {
public:
	Plane() = default;
	Plane(const Vector3 &p, const Vector3 &n, const Matrix4 *transform = nullptr) { Set(p, n, transform); }

	/**
		@short Return point distance to plane.
		Distance is signed, positive when the point is in front of the plane,
		negative otherwise.
	*/
	float DistanceToPlane(const Vector3 &p) const;

	/// Set plane from point/normal and an optional transformation matrix.
	void Set(const Vector3 &p, const Vector3 &n, const Matrix4 *transform = nullptr);
	/// Set plane three vectors and an optional transformation matrix.
	void Set(const Vector3 p[3], const Matrix4 *transform = nullptr);

	Vector3 n{0, 0, 0};
	float d{0};
};

} // namespace hg
