// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/math.h"
#include "foundation/vector4.h"

namespace hg {

struct Vector3;

/// Matrix 4x4
class Matrix44 {
public:
	static const Matrix44 Zero;
	static const Matrix44 Identity;

	Matrix44() = default;
	Matrix44(float m00, float m10, float m20, float m30,
		float m01, float m11, float m21, float m31,
		float m02, float m12, float m22, float m32,
		float m03, float m13, float m23, float m33) {
		m[0][0] = m00;
		m[1][0] = m10;
		m[2][0] = m20;
		m[3][0] = m30;
		m[0][1] = m01;
		m[1][1] = m11;
		m[2][1] = m21;
		m[3][1] = m31;
		m[0][2] = m02;
		m[1][2] = m12;
		m[2][2] = m22;
		m[3][2] = m32;
		m[0][3] = m03;
		m[1][3] = m13;
		m[2][3] = m23;
		m[3][3] = m33;
	}

	void Transform(Vector4 *__restrict out, const Vector3 *__restrict in, uint32_t count = 1);
	void Transform(Vector4 *__restrict out, const Vector4 *__restrict in, uint32_t count = 1);

	/// Inverse matrix 4x4.
	Matrix44 Inverse(bool &result) const;
	/// Inverse matrix 4x4, this variant does not report error and might silently fail.
	Matrix44 Inverse() const;

	inline Vector4 GetRow(uint32_t n) const { return Vector4(m[0][n], m[1][n], m[2][n], m[3][n]); }
	inline Vector4 GetColumn(uint32_t n) const { return Vector4(m[n][0], m[n][1], m[n][2], m[n][3]); }

	inline void SetRow(uint32_t n, const Vector4 &v) {
		m[0][n] = v.x;
		m[1][n] = v.y;
		m[2][n] = v.z;
		m[3][n] = v.w;
	}
	inline void SetColumn(uint32_t n, const Vector4 &v) {
		m[n][0] = v.x;
		m[n][1] = v.y;
		m[n][2] = v.z;
		m[n][3] = v.w;
	}

	float m[4][4];
};

inline Matrix44 operator*(const Matrix44 &a, const Matrix44 &b) {
#define __M44M44(__I, __J) a.m[__I][0] * b.m[0][__J] + a.m[__I][1] * b.m[1][__J] + a.m[__I][2] * b.m[2][__J] + a.m[__I][3] * b.m[3][__J]
	return Matrix44(__M44M44(0, 0), __M44M44(1, 0), __M44M44(2, 0), __M44M44(3, 0), __M44M44(0, 1), __M44M44(1, 1), __M44M44(2, 1), __M44M44(3, 1),
		__M44M44(0, 2), __M44M44(1, 2), __M44M44(2, 2), __M44M44(3, 2), __M44M44(0, 3), __M44M44(1, 3), __M44M44(2, 3), __M44M44(3, 3));
}

class Matrix4;

Matrix44 operator*(const Matrix44 &a, const Matrix4 &b);
Matrix44 operator*(const Matrix4 &a, const Matrix44 &b);

Vector3 operator*(const Vector3 &v, const Matrix44 &m);
Vector4 operator*(const Vector4 &v, const Matrix44 &m);

} // namespace hg
