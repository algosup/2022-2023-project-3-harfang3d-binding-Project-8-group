// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/math.h"
#include <cstddef>

namespace hg {

template <typename T>
struct tVector2;
struct Vector4;

/// 3-Component vector
struct Vector3 {
	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 Left, Right, Up, Down, Front, Back;

	Vector3() = default;
	Vector3(float x, float y, float z);

	explicit Vector3(const tVector2<int> &v);
	explicit Vector3(const tVector2<float> &v);
	explicit Vector3(const Vector4 &v);

	inline Vector3 &operator+=(const Vector3 &b) {
		x += b.x;
		y += b.y;
		z += b.z;
		return *this;
	}
	inline Vector3 &operator+=(const float k) {
		x += k;
		y += k;
		z += k;
		return *this;
	}
	inline Vector3 &operator-=(const Vector3 &b) {
		x -= b.x;
		y -= b.y;
		z -= b.z;
		return *this;
	}
	inline Vector3 &operator-=(const float k) {
		x -= k;
		y -= k;
		z -= k;
		return *this;
	}
	inline Vector3 &operator*=(const Vector3 &b) {
		x *= b.x;
		y *= b.y;
		z *= b.z;
		return *this;
	}
	inline Vector3 &operator*=(const float k) {
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}
	inline Vector3 &operator/=(const Vector3 &b) {
		x /= b.x;
		y /= b.y;
		z /= b.z;
		return *this;
	}
	inline Vector3 &operator/=(const float k) {
		float k_ = k ? 1 / k : 0;
		x *= k_;
		y *= k_;
		z *= k_;
		return *this;
	}

	inline float operator[](size_t n) const { return (&x)[n]; }
	inline float &operator[](size_t n) { return (&x)[n]; }

	/// Reverse this vector.
	inline void Reverse() {
		x = -x;
		y = -y;
		z = -z;
	}
	/// Inverse vector.
	inline void Inverse() {
		x = x ? 1.f / x : 0;
		y = y ? 1.f / y : 0;
		z = z ? 1.f / z : 0;
	}

	/// Normalize this vector.
	inline void Normalize() {
		float l = Len();
		if (l) {
			float k = 1.f / l;
			x *= k;
			y *= k;
			z *= k;
		}
	}

	/// Normalize vector.
	inline Vector3 Normalized() const {
		float l = Len();
		float k = l ? 1.f / l : 1.f;
		return Vector3(x * k, y * k, z * k);
	}

	/// Clamp vector components to [min;max].
	Vector3 Clamped(float min, float max) const;
	/// Clamp vector components to [min;max].
	Vector3 Clamped(const Vector3 &min, const Vector3 &max) const;
	/// Clamp vector magnitude to [min;max].
	Vector3 ClampedMagnitude(float min, float max) const;

	/// Return the opposite vector to this vector.
	inline Vector3 Reversed() const { return Vector3(-x, -y, -z); }
	/// Return the inverse vector to this vector.
	inline Vector3 Inversed() const { return Vector3(1.f / x, 1.f / y, 1.f / z); }

	/// Absolute vector.
	inline Vector3 Abs() const { return Vector3(x > 0 ? x : -x, y > 0 ? y : -y, z > 0 ? z : -z); }
	/// Sign vector.
	inline Vector3 Sign() const { return Vector3(x < 0.f ? -1.f : 1.f, y < 0.f ? -1.f : 1.f, z < 0.f ? -1.f : 1.f); }

	/// Maximum of two vectors.
	static Vector3 Maximum(const Vector3 &a, const Vector3 &b) { return Vector3(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z); }
	/// Minimum of two vectors.
	static Vector3 Minimum(const Vector3 &a, const Vector3 &b) { return Vector3(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z); }

	/// Squared vector length.
	inline float Len2() const { return static_cast<float>(x * x + y * y + z * z); }
	/// Vector length.
	inline float Len() const { return Sqrt(static_cast<float>(x * x + y * y + z * z)); }

	Vector3 Floor() const;
	Vector3 Ceil() const;

	/**
		@short Return a vector which is facing a given direction.
		Returns a copy of this vector if it is already facing the given
		direction or the opposite of this vector otherwise.
	*/
	Vector3 FaceForward(Vector3 &dir);

	float x, y, z;
};

} // namespace hg
