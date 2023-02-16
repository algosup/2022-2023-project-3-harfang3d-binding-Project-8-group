// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/math.h"

namespace hg {

class Matrix4;

template <typename T>
struct tVector2;
struct Vector3;
struct Vector4;

/**
	@short Matrix 3x3
	This matrix class is column major.
*/
class Matrix3 {
public:
	static const Matrix3 Zero;
	static const Matrix3 Identity;

	Matrix3() = default;
	Matrix3(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22);
	explicit Matrix3(const Matrix4 &m);
	explicit Matrix3(const float *v);
	Matrix3(const Vector3 &x, const Vector3 &y, const Vector3 &z);

	//
	bool operator==(const Matrix3 &b) const {
		for (uint32_t i = 0; i < 3; i++)
			for (uint32_t j = 0; j < 3; j++)
				if (m[i][j] != b.m[i][j])
					return false;
		return true;
	}

	bool operator!=(const Matrix3 &b) const {
		for (uint32_t i = 0; i < 3; ++i)
			for (uint32_t j = 0; j < 3; ++j)
				if (m[i][j] != b.m[i][j])
					return true;
		return false;
	}

	//
	Matrix3 operator+(const Matrix3 &b) const {
		Matrix3 r;
		for (uint32_t j = 0; j < 3; ++j)
			for (uint32_t i = 0; i < 3; ++i)
				r.m[i][j] = m[i][j] + b.m[i][j];
		return r;
	}

	Matrix3 operator-(const Matrix3 &b) const {
		Matrix3 r;
		for (uint32_t j = 0; j < 3; ++j)
			for (uint32_t i = 0; i < 3; ++i)
				r.m[i][j] = m[i][j] - b.m[i][j];
		return r;
	}

	Matrix3 operator*(const float v) const {
		Matrix3 r;
		for (uint32_t j = 0; j < 3; ++j)
			for (uint32_t i = 0; i < 3; ++i)
				r.m[i][j] = m[i][j] * v;
		return r;
	}

	tVector2<float> operator*(const tVector2<float> &v) const;
	Vector3 operator*(const Vector3 &v) const;
	Vector4 operator*(const Vector4 &v) const;

	Matrix3 operator*(const Matrix3 &b) const {
#define __M33M33(__I, __J) m[__I][0] * b.m[0][__J] + m[__I][1] * b.m[1][__J] + m[__I][2] * b.m[2][__J]
		return Matrix3(__M33M33(0, 0), __M33M33(1, 0), __M33M33(2, 0), __M33M33(0, 1), __M33M33(1, 1), __M33M33(2, 1), __M33M33(0, 2), __M33M33(1, 2),
			__M33M33(2, 2));
	}

	Matrix3 &operator+=(const Matrix3 &b) {
		*this = *this + b;
		return *this;
	}

	Matrix3 &operator-=(const Matrix3 &b) {
		*this = *this - b;
		return *this;
	}

	Matrix3 &operator*=(const float k) {
		for (uint32_t j = 0; j < 3; ++j)
			for (uint32_t i = 0; i < 3; ++i)
				m[i][j] *= k;
		return *this;
	}

	Matrix3 &operator*=(const Matrix3 &b) {
		*this = (*this) * b;
		return *this;
	}

	/// Transform an array of vector objects.
	void Transform(tVector2<float> *__restrict out, const tVector2<float> *__restrict in, uint32_t count = 1) const __restrict;
	void Transform(Vector3 *__restrict out, const Vector3 *__restrict in, uint32_t count = 1) const __restrict;
	void Transform(Vector4 *__restrict out, const Vector4 *__restrict in, uint32_t count = 1) const __restrict;

	/// Compute the determinant of the matrix.
	float Det() const {
		return ((m[1][1] * m[2][2]) - (m[1][2] * m[2][1])) * m[0][0] + ((m[1][2] * m[2][0]) - (m[1][0] * m[2][2])) * m[0][1] +
			   ((m[1][0] * m[2][1]) - (m[1][1] * m[2][0])) * m[0][2];
	}

	/// Compute inverse matrix.
	bool Inverse(Matrix3 &i) const;

	/// Return the transposed matrix.
	inline Matrix3 Transposed() const { return Matrix3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]); }

	/// Return the nth row.
	Vector3 GetRow(uint32_t n) const;
	/// Return the nth column.
	Vector3 GetColumn(uint32_t n) const;
	/// Set the nth row.
	void SetRow(uint32_t n, const Vector3 &row);
	/// Set the nth column.
	void SetColumn(uint32_t n, const Vector3 &col);

	Vector3 GetX() const;
	Vector3 GetY() const;
	Vector3 GetZ() const;
	Vector3 GetTranslation() const;
	Vector3 GetScale() const;

	void SetX(const Vector3 &);
	void SetY(const Vector3 &);
	void SetZ(const Vector3 &);
	void SetTranslation(const tVector2<float> &);
	void SetTranslation(const Vector3 &);
	void SetScale(const Vector3 &);

	void Set(const Vector3 &x, const Vector3 &y, const Vector3 &z);

	/// Return this matrix after normalization.
	Matrix3 Normalized() const;
	/// Normalize as orthonormal base.
	Matrix3 Orthonormalized() const;
	/// Return an Euler orientation equivalent to this matrix.
	Vector3 ToEuler(RotationOrder rorder = RotationOrder_Default) const;

	/// Vector matrix.
	static Matrix3 VectorMatrix(const Vector3 &v);
	/// Translation matrix.
	static Matrix3 TranslationMatrix(const tVector2<float> &t);
	static Matrix3 TranslationMatrix(const Vector3 &t);
	/// Scale matrix.
	static Matrix3 ScaleMatrix(const tVector2<float> &s);
	static Matrix3 ScaleMatrix(const Vector3 &s);
	/// Cross product matrix.
	static Matrix3 CrossProductMatrix(const Vector3 &v);

	/// Rotation matrix around X axis.
	static Matrix3 RotationMatrixXAxis(float angle);
	/// Rotation matrix around Y axis.
	static Matrix3 RotationMatrixYAxis(float angle);
	/// Rotation matrix around Z axis.
	static Matrix3 RotationMatrixZAxis(float angle);

	/// Rotation matrix in 2D around a pivot point.
	static Matrix3 RotationMatrix2D(float angle, const tVector2<float> &pivot);

	/// From Euler triplet.
	static Matrix3 FromEuler(float x = 0, float y = 0, float z = 0, RotationOrder rorder = RotationOrder_Default);
	/// From Euler vector.
	static Matrix3 FromEuler(const Vector3 &euler, RotationOrder rorder = RotationOrder_Default);

	/**
		@short Look at.
		Create a rotation matrix pointing in the same direction as a vector.
		The up vector in world space of the final matrix can be specified.
	*/
	static Matrix3 LookAt(const Vector3 &front);
	static Matrix3 LookAt(const Vector3 &front, const Vector3 &up);

	float m[3][3];
};

Matrix3 RotationMatrixXZY(float x, float y, float z);
Matrix3 RotationMatrixZYX(float x, float y, float z);
Matrix3 RotationMatrixXYZ(float x, float y, float z);
Matrix3 RotationMatrixZXY(float x, float y, float z);
Matrix3 RotationMatrixYZX(float x, float y, float z);
Matrix3 RotationMatrixYXZ(float x, float y, float z);
Matrix3 RotationMatrixXY(float x, float y);

} // namespace hg
