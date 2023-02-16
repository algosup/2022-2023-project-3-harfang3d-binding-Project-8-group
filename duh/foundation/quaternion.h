// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/allocator.h"
#include "foundation/math.h"

namespace hg {

struct Vector3;
class Matrix3;

/// Quaternion
struct Quaternion {
	Quaternion(float _x = 0, float _y = 0, float _z = 0, float _w = 1.f) : x(_x), y(_y), z(_z), w(_w) {}
	Quaternion(const Quaternion &q) : x(q.x), y(q.y), z(q.z), w(q.w) {}

	Quaternion &operator+=(const Quaternion &b) {
		x += b.x;
		y += b.y;
		z += b.z;
		w += b.w;
		return *this;
	}

	Quaternion &operator+=(float k) {
		x += k;
		y += k;
		z += k;
		w += k;
		return *this;
	}

	Quaternion &operator-=(const Quaternion &b) {
		x -= b.x;
		y -= b.y;
		z -= b.z;
		w -= b.w;
		return *this;
	}

	Quaternion &operator-=(float k) {
		x -= k;
		y -= k;
		z -= k;
		w -= k;
		return *this;
	}

	Quaternion &operator*=(const Quaternion &b) {
		Quaternion t = *this;
		w = t.w * b.w - (t.x * b.x + t.y * b.y + t.z * b.z);
		x = t.w * b.x + b.w * t.x + t.y * b.z - t.z * b.y;
		y = t.w * b.y + b.w * t.y + t.z * b.x - t.x * b.z;
		z = t.w * b.z + b.w * t.z + t.x * b.y - t.y * b.x;
		return *this;
	}

	Quaternion &operator*=(float k) {
		x *= k;
		y *= k;
		z *= k;
		w *= k;
		return *this;
	}

	Quaternion &operator/=(float k) {
		k = 1.f / k;
		x *= k;
		y *= k;
		z *= k;
		w *= k;
		return *this;
	}

	Quaternion &operator=(const Quaternion &b) {
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
		return *this;
	}

	Quaternion operator+(const Quaternion &b) const { return Quaternion(x + b.x, y + b.y, z + b.z, w + b.w); }
	Quaternion operator+(float v) const { return Quaternion(x + v, y + v, z + v, w + v); }
	Quaternion operator-(const Quaternion &b) const { return Quaternion(x - b.x, y - b.y, z - b.z, w - b.w); }
	Quaternion operator-(float v) const { return Quaternion(x - v, y - v, z - v, w - v); }

	Quaternion operator*(const Quaternion &b) const {
		return Quaternion(w * b.x + b.w * x + y * b.z - z * b.y, w * b.y + b.w * y + z * b.x - x * b.z, w * b.z + b.w * z + x * b.y - y * b.x,
			w * b.w - (x * b.x + y * b.y + z * b.z));
	}

	Quaternion operator*(float v) const { return Quaternion(x * v, y * v, z * v, w * v); }
	Quaternion operator/(float v) const {
		v = 1.f / v;
		return Quaternion(x * v, y * v, z * v, w * v);
	}

	/// Dot product.
	inline float Dot(const Quaternion &b) const { return x * b.x + y * b.y + z * b.z + w * b.w; }

	/// Normalize quaternion.
	Quaternion Normalized() const;
	/// Inverse quaternion.
	Quaternion Inversed() const;
	/// To rotation matrix.
	Matrix3 ToMatrix3() const;
	/// To Euler angle triplet.
	Vector3 ToEuler(RotationOrder = RotationOrder_Default) const;

	/// Distance to quaternion.
	static float Distance(const Quaternion &a, const Quaternion &b);
	/// Slerp.
	static Quaternion Slerp(float t, const Quaternion &a, const Quaternion &b);

	/// From Euler angle triplet.
	static Quaternion FromEuler(float x, float y, float z, RotationOrder = RotationOrder_Default);
	static Quaternion FromEuler(const Vector3 &v, RotationOrder = RotationOrder_Default);
	/// Get an orientation from a 'look at' vector (look_at = to - from).
	static Quaternion LookAt(const Vector3 &at);
	/// From matrix3.
	static Quaternion FromMatrix3(const Matrix3 &m);
	/// From axis-angle.
	static Quaternion FromAxisAngle(float angle, const Vector3 &axis);

	float x, y, z, w;
};

inline bool operator==(const Quaternion &a, const Quaternion &b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
inline bool operator!=(const Quaternion &a, const Quaternion &b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }

} // namespace hg
