// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstddef>
#include "foundation/math.h"

namespace hg {

template <typename T>
struct tVector2;
struct Vector3;
struct Color;

class Matrix4;

struct Vector4 {
	static const Vector4 Zero;
	static const Vector4 One;

	Vector4() = default;
	Vector4(float x, float y, float z, float w);

	explicit Vector4(const tVector2<int> &v);
	explicit Vector4(const tVector2<float> &v);
	explicit Vector4(const Vector3 &v);

	explicit Vector4(const Color &c);

	inline bool operator==(const Vector4 &b) const { return x == b.x && y == b.y && z == b.z; }
	inline bool operator!=(const Vector4 &b) const { return x != b.x || y != b.y || z != b.z; }

	inline Vector4 &operator+=(const Vector4 &b) {
		x += b.x;
		y += b.y;
		z += b.z;
		w += b.w;
		return *this;
	}

	inline Vector4 &operator+=(const float k) {
		x += k;
		y += k;
		z += k;
		w += k;
		return *this;
	}

	inline Vector4 &operator-=(const Vector4 &b) {
		x -= b.x;
		y -= b.y;
		z -= b.z;
		w -= b.w;
		return *this;
	}

	inline Vector4 &operator-=(const float k) {
		x -= k;
		y -= k;
		z -= k;
		w -= k;
		return *this;
	}

	inline Vector4 &operator*=(const Vector4 &b) {
		x *= b.x;
		y *= b.y;
		z *= b.z;
		w *= b.w;
		return *this;
	}

	inline Vector4 &operator*=(const float k) {
		x *= k;
		y *= k;
		z *= k;
		w *= k;
		return *this;
	}

	inline Vector4 &operator/=(const Vector4 &b) {
		x /= b.x;
		y /= b.y;
		z /= b.z;
		w /= b.w;
		return *this;
	}

	inline Vector4 &operator/=(const float k) {
		float k_ = k ? 1 / k : 0;
		x *= k_;
		y *= k_;
		z *= k_;
		w *= k_;
		return *this;
	}

	inline Vector4 operator+(const Vector4 &b) const { return Vector4(x + b.x, y + b.y, z + b.z, w + b.w); }
	inline Vector4 operator+(const float v) const { return Vector4(x + v, y + v, z + v, w + v); }
	inline Vector4 operator-(const Vector4 &b) const { return Vector4(x - b.x, y - b.y, z - b.z, w - b.w); }
	inline Vector4 operator-(const float v) const { return Vector4(x - v, y - v, z - v, w - v); }
	inline Vector4 operator*(const Vector4 &b) const { return Vector4(x * b.x, y * b.y, z * b.z, w * b.w); }
	inline Vector4 operator*(const float v) const { return Vector4(x * v, y * v, z * v, w * v); }
	inline Vector4 operator/(const Vector4 &b) const { return Vector4(x / b.x, y / b.y, z / b.z, w / b.w); }
	inline Vector4 operator/(const float v) const {
		float i = v ? 1.f / v : 0.f;
		return Vector4(x * i, y * i, z * i, w * i);
	}

	void operator*=(const Matrix4 &);
	Vector4 operator*(const Matrix4 &)const;

	inline float operator[](size_t n) const { return (&x)[n]; }
	inline float &operator[](size_t n) { return (&x)[n]; }

	inline Vector4 Abs() const { return Vector4(x > 0 ? x : -x, y > 0 ? y : -y, z > 0 ? z : -z, w > 0 ? w : -w); }

	inline Vector4 Normalized() const {
		const float l = Sqrt(x * x + y * y + z * z + w * w);
		return l > 0.f ? *this / l : *this;
	}

	float x, y, z, w;
};

} // namespace hg
