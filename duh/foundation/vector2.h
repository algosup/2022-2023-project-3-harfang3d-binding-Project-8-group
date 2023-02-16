// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/math.h"

namespace hg {

struct Vector3;
struct Vector4;

class Matrix3;

template <class T>
struct tVector2 {
	static tVector2 Zero;
	static tVector2 One;

	tVector2<T>() = default;
	tVector2<T>(T a, T b) : x(a), y(b) {}

	explicit tVector2<T>(const Vector3 &v);
	explicit tVector2<T>(const Vector4 &v);

	inline bool operator==(const tVector2<T> &b) const { return x == b.x && y == b.y; }
	inline bool operator!=(const tVector2<T> &b) const { return x != b.x || y != b.y; }

	inline tVector2<T> &operator+=(const tVector2<T> &b) {
		x += b.x;
		y += b.y;
		return *this;
	}

	inline tVector2<T> &operator+=(const T v) {
		x += v;
		y += v;
		return *this;
	}

	inline tVector2<T> &operator-=(const tVector2<T> &b) {
		x -= b.x;
		y -= b.y;
		return *this;
	}

	inline tVector2<T> &operator-=(const T v) {
		x -= v;
		y -= v;
		return *this;
	}

	inline tVector2<T> &operator*=(const tVector2<T> &b) {
		x *= b.x;
		y *= b.y;
		return *this;
	}

	inline tVector2<T> &operator*=(const T v) {
		x *= v;
		y *= v;
		return *this;
	}

	inline tVector2<T> &operator/=(const tVector2<T> &b) {
		x /= b.x;
		y /= b.y;
		return *this;
	}

	inline tVector2<T> &operator/=(const T v) {
		x /= v;
		y /= v;
		return *this;
	}

	inline tVector2<T> operator+(const tVector2<T> &b) const { return tVector2<T>(x + b.x, y + b.y); }
	inline tVector2<T> operator+(const T v) const { return tVector2<T>(x + v, y + v); }
	inline tVector2<T> operator-(const tVector2<T> &b) const { return tVector2<T>(x - b.x, y - b.y); }
	inline tVector2<T> operator-(const T v) const { return tVector2<T>(x - v, y - v); }
	inline tVector2<T> operator*(const tVector2<T> &b) const { return tVector2<T>(x * b.x, y * b.y); }
	inline tVector2<T> operator*(const T v) const { return tVector2<T>(x * v, y * v); }
	inline tVector2<T> operator/(const tVector2<T> &b) const { return tVector2<T>(x / b.x, y / b.y); }
	inline tVector2<T> operator/(const T v) const { return tVector2<T>(x / v, y / v); }

	tVector2<T> operator*(const Matrix3 &m) const;

	tVector2<T> Min(const tVector2<T> &v) const { return tVector2<T>(x < v.x ? x : v.x, y < v.y ? y : v.y); }
	tVector2<T> Max(const tVector2<T> &v) const { return tVector2<T>(x > v.x ? x : v.x, y > v.y ? y : v.y); }

	/// Squared vector length.
	inline T Len2() const { return x * x + y * y; }
	/// Vector length.
	inline T Len() const { return T(Sqrt(float(Len2()))); }

	/// Dot product.
	inline T Dot(const tVector2<T> &b) const { return x * b.x + y * b.y; }

	/// Normalize this vector.
	inline void Normalize() {
		if (auto l = Len()) {
			x = x / l;
			y = y / l;
		}
	}

	/// Normalize vector.
	inline tVector2<T> Normalized() const {
		auto l = Len();
		return !l ? tVector2<T>(0, 0) : tVector2<T>(x / l, y / l);
	}

	/// Reversed vector.
	inline tVector2<T> Reversed() const { return tVector2<T>(-x, -y); }

	/// Vector squared distance.
	static inline T Dist2(const tVector2 &a, const tVector2 &b) { return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y); }
	/// Vector distance.
	static inline T Dist(const tVector2 &a, const tVector2 &b) { return T(Sqrt(float(Dist2(a, b)))); }

	T x, y;
};

typedef tVector2<float> Vector2;
typedef tVector2<int> iVector2;

template <typename T>
tVector2<T> tVector2<T>::Zero(0, 0);
template <typename T>
tVector2<T> tVector2<T>::One(1, 1);

template <typename T>
bool AlmostEqual(const tVector2<T> &a, const tVector2<T> &b, float e) { return Abs(a.x - b.x) <= e && Abs(a.y - b.y) <= e; }

} // namespace hg
