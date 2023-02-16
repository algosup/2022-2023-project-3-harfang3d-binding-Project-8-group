// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/math.h"
#include "foundation/vector3.h"
#include "foundation/vector4.h"

namespace hg {

struct Vector3;
struct Vector4;
class Matrix3;

// Matrix 4x3 (for affine transform)
class Matrix4 {
public:
	static const Matrix4 Zero;
	static const Matrix4 Identity;

	Matrix4() = default;
	Matrix4(float m00, float m10, float m20,
		float m01, float m11, float m21,
		float m02, float m12, float m22,
		float m03, float m13, float m23) {
		Set(m00, m10, m20,
			m01, m11, m21,
			m02, m12, m22,
			m03, m13, m23);
	}

	explicit Matrix4(const float *v) { Set(v); }
	explicit Matrix4(const Matrix3 &m);

	bool operator==(const Matrix4 &b) const {
		for (size_t j = 0; j < 3; ++j)
			for (size_t i = 0; i < 4; ++i)
				if (m[j][i] != b.m[j][i])
					return false;
		return true;
	}

	bool operator!=(const Matrix4 &b) const {
		for (size_t j = 0; j < 3; ++j)
			for (size_t i = 0; i < 4; ++i)
				if (m[j][i] != b.m[j][i])
					return true;
		return false;
	}

	inline Matrix4 operator*(const Matrix4 &b) const {
		return Matrix4(
			m[0][0] * b.m[0][0] + m[0][1] * b.m[1][0] + m[0][2] * b.m[2][0],
			m[1][0] * b.m[0][0] + m[1][1] * b.m[1][0] + m[1][2] * b.m[2][0],
			m[2][0] * b.m[0][0] + m[2][1] * b.m[1][0] + m[2][2] * b.m[2][0],

			m[0][0] * b.m[0][1] + m[0][1] * b.m[1][1] + m[0][2] * b.m[2][1],
			m[1][0] * b.m[0][1] + m[1][1] * b.m[1][1] + m[1][2] * b.m[2][1],
			m[2][0] * b.m[0][1] + m[2][1] * b.m[1][1] + m[2][2] * b.m[2][1],

			m[0][0] * b.m[0][2] + m[0][1] * b.m[1][2] + m[0][2] * b.m[2][2],
			m[1][0] * b.m[0][2] + m[1][1] * b.m[1][2] + m[1][2] * b.m[2][2],
			m[2][0] * b.m[0][2] + m[2][1] * b.m[1][2] + m[2][2] * b.m[2][2],

			m[0][0] * b.m[0][3] + m[0][1] * b.m[1][3] + m[0][2] * b.m[2][3] + m[0][3],
			m[1][0] * b.m[0][3] + m[1][1] * b.m[1][3] + m[1][2] * b.m[2][3] + m[1][3],
			m[2][0] * b.m[0][3] + m[2][1] * b.m[1][3] + m[2][2] * b.m[2][3] + m[2][3]);
	}

	const Matrix4 operator*(float v) const {
		Matrix4 o;
		for (uint32_t j = 0; j < 3; ++j)
			for (uint32_t i = 0; i < 4; ++i)
				o.m[j][i] = m[j][i] * v;
		return o;
	}

	const Matrix4 operator+(const Matrix4 &b) const {
		Matrix4 o;
		for (size_t j = 0; j < 3; ++j)
			for (size_t i = 0; i < 4; ++i)
				o.m[j][i] = m[j][i] + b.m[j][i];
		return o;
	}

	const Matrix4 operator-(const Matrix4 &b) const {
		Matrix4 o;
		for (size_t j = 0; j < 3; ++j)
			for (size_t i = 0; i < 4; ++i)
				o.m[j][i] = m[j][i] - b.m[j][i];
		return o;
	}

	Vector3 operator*(const Vector3 &v) const;
	Vector4 operator*(const Vector4 &v) const;

	/// Return the nth row.
	Vector3 GetRow(uint32_t n) const;
	/// Return the nth column.
	Vector4 GetColumn(uint32_t n) const;
	/// Set the nth row.
	void SetRow(uint32_t n, const Vector3 &v);
	/// Set the nth column.
	void SetColumn(uint32_t n, const Vector4 &v);

	Vector3 GetX() const;
	Vector3 GetY() const;
	Vector3 GetZ() const;
	Vector3 GetT() const;
	Vector3 GetTranslation() const;
	Vector3 GetRotation(RotationOrder order = RotationOrder_Default) const;
	Matrix3 GetRotationMatrix() const;
	Vector3 GetScale() const;

	void SetX(const Vector3 &);
	void SetY(const Vector3 &);
	void SetZ(const Vector3 &);
	void SetTranslation(const Vector3 &);
	void SetScale(const Vector3 &);

	/// Inverse matrix.
	bool Inverse(Matrix4 &out) const;
	/// Return the inverse of an affine transformation matrix.
	Matrix4 InversedFast() const;

	/// Normalize matrix.
	Matrix4 Orthonormalized() const;

	/// Interpolate between two 4x4 transformation matrices.
	static Matrix4 LerpAsOrthonormalBase(const Matrix4 &a, const Matrix4 &b, float k, bool fast = false);

	/// Decompose a transformation matrix into a position vector, a scale vector and a 3x3 rotation matrix.
	void Decompose(Vector3 *position, Vector3 *scale = nullptr, Matrix3 *rotation = nullptr) const;
	/// Decompose a transformation matrix into a position vector, a scale vector and a rotation vector.
	void Decompose(Vector3 *position, Vector3 *scale, Vector3 *rotation, RotationOrder order = RotationOrder_Default) const;

	/// Transform a series of vector.
	void Transform(Vector3 *__restrict out, const Vector3 *__restrict in, uint32_t count = 1) const __restrict;
	/// Transform a series of vector.
	void Transform(Vector4 *__restrict out, const Vector3 *__restrict in, uint32_t count = 1) const __restrict;

	/// Transform a vector array by the matrix upper-left 3x3 matrix.
	void Rotate(Vector3 *__restrict out, const Vector3 *__restrict in, uint32_t count = 1) const __restrict;

	/**
		@short Look at.
		Return this matrix modified so that it looks toward a world space position.
		The up vector in world space of the final matrix can be specified.
	*/
	Matrix4 LookAt(const Vector3 &at) const;
	Matrix4 LookAt(const Vector3 &at, const Vector3 &up) const;

	/// Set values.
	inline void Set(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22, float m03, float m13, float m23) {
		m[0][0] = m00;
		m[1][0] = m10;
		m[2][0] = m20;
		m[0][1] = m01;
		m[1][1] = m11;
		m[2][1] = m21;
		m[0][2] = m02;
		m[1][2] = m12;
		m[2][2] = m22;
		m[0][3] = m03;
		m[1][3] = m13;
		m[2][3] = m23;
	}

	/// Set from float array (ordered per column).
	void Set(const float *m);

	/// Translation matrix.
	static Matrix4 TranslationMatrix(const Vector3 &t);
	/// Rotation matrix.
	static Matrix4 RotationMatrix(const Vector3 &euler, RotationOrder rorder = RotationOrder_Default);
	/// Scale matrix.
	static Matrix4 ScaleMatrix(const Vector3 &s);
	static Matrix4 ScaleMatrix(float s);
	/// Position/scale/rotation matrix.
	static Matrix4 TransformationMatrix(const Vector3 &p, const Vector3 &r, const Vector3 &s = Vector3::One);
	/// Position/scale/rotation matrix.
	static Matrix4 TransformationMatrix(const Vector3 &p, const Matrix3 &r, const Vector3 &s = Vector3::One);
	/// Create a look toward matrix.
	static Matrix4 LookToward(const Vector3 &pos, const Vector3 &dir, const Vector3 &scl = Vector3::One);
	static Matrix4 LookTowardUp(const Vector3 &pos, const Vector3 &dir, const Vector3 &up, const Vector3 &scl = Vector3::One);

	float m[3][4];
};

} // namespace hg
