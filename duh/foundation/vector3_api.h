// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/vector3.h"
#include "foundation/math.h"

namespace hg {

inline bool operator==(const Vector3 &a, const Vector3 &b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
inline bool operator!=(const Vector3 &a, const Vector3 &b) { return a.x != b.x || a.y != b.y || a.z != b.z; }

inline Vector3 operator+(const Vector3 &a, const Vector3 &b) { return Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vector3 operator+(const Vector3 &a, const float v) { return Vector3(a.x + v, a.y + v, a.z + v); }
inline Vector3 operator-(const Vector3 &a, const Vector3 &b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline Vector3 operator-(const Vector3 &a, const float v) { return Vector3(a.x - v, a.y - v, a.z - v); }
inline Vector3 operator*(const Vector3 &a, const Vector3 &b) { return Vector3(a.x * b.x, a.y * b.y, a.z * b.z); }
inline Vector3 operator*(const Vector3 &a, const float v) { return Vector3(a.x * v, a.y * v, a.z * v); }
inline Vector3 operator*(const float v, const Vector3 &a) { return Vector3(a.x * v, a.y * v, a.z * v); }
inline Vector3 operator/(const Vector3 &a, const Vector3 &b) { return Vector3(a.x / b.x, a.y / b.y, a.z / b.z); }
inline Vector3 operator/(const Vector3 &a, const float v) {
	float i = v ? 1.f / v : 0.f;
	return Vector3(a.x * i, a.y * i, a.z * i);
}

class Matrix3;
Vector3 operator*(const Vector3 &a, const Matrix3 &m);
class Matrix4;
Vector3 operator*(const Vector3 &a, const Matrix4 &m);

/// Return a random vector.
Vector3 RandomVector3(float min = -1, float max = 1);
Vector3 RandomVector3(const Vector3 &min, const Vector3 &max);

inline bool AlmostEqual(const Vector3 &a, const Vector3 &b, float epsilon) { return Abs(a.x - b.x) < epsilon && Abs(a.y - b.y) < epsilon && Abs(a.z - b.z) < epsilon; }

/**
	@short Vector base to Euler.
	@note base convention u = {0,0,1}, v = {1,0,0}, second axis is optional.
*/
Vector3 BaseToEuler(const Vector3 &u);
Vector3 BaseToEuler(const Vector3 &u, const Vector3 &v);

/// Return hash of a vector.S
int Hash(const Vector3 &v);

/// Vector squared distance.
inline float Dist2(const Vector3 &a, const Vector3 &b) { return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z); }
/// Vector distance.
inline float Dist(const Vector3 &a, const Vector3 &b) { return Sqrt(Dist2(a, b)); }

/// Dot product.
inline float Dot(const Vector3 &a, const Vector3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
/// Cross product.
inline Vector3 Cross(const Vector3 &a, const Vector3 &b) { return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

/// Return a unit vector reflected by a normal vector.
inline Vector3 Reflect(const Vector3 &v, const Vector3 &n) {
	const Vector3 rv = v.Reversed();
	return n * (2.f * Dot(rv, n)) - rv;
}

/// Return a unit vector refracted around a normal vector.S
inline Vector3 Refract(const Vector3 &v, const Vector3 &n, float k_in = 1, float k_out = 1) {
	const float k = k_in / k_out;
	return v * k + n * (k - 1.f);
}

struct Vector4;
Vector3 Vector3FromVector4(const Vector4 &v);

} // namespace hg
