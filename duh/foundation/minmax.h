// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/vector3.h"
#include "foundation/vector3_api.h"

namespace hg {

struct MinMax {
	MinMax() : mn(0, 0, 0), mx(0, 0, 0) {}
	MinMax(const Vector3 &min, const Vector3 &max) : mn(min), mx(max) {}

	/// Get the min-max area.
	inline float GetArea() { return (mx.x - mn.x) * (mx.y - mn.y) * (mx.z - mn.z); }
	/// Return the minmax center.
	inline Vector3 GetCenter() { return (mn + mx) * 0.5f; }

	Vector3 mn, mx;
};

/// Fill an array of vector3 with the position of the minmax vertices.
void GetMinMaxVertices(const MinMax &minmax, Vector3 out[8]);
/// Return the origin and radius of the minmax bounding sphere.
void ComputeMinMaxBoundingSphere(const MinMax &minmax, Vector3 &origin, float &radius);

/// Test minmax overlap on a specific axis.
inline bool Overlap(const MinMax &a, const MinMax &b, Axis axis) { return !(b.mn[axis] > a.mx[axis] || b.mx[axis] < a.mn[axis]); }
/// Return whether two MinMax overlap at a given time.
inline bool Overlap(const MinMax &a, const MinMax &b) { return !(a.mx.x < b.mn.x || a.mx.y < b.mn.y || a.mx.z < b.mn.z || b.mx.x < a.mn.x || b.mx.y < a.mn.y || b.mx.z < a.mn.z); }

inline bool operator==(const MinMax &a, const MinMax &b) { return a.mn == b.mn && a.mx == b.mx; }
inline bool operator!=(const MinMax &a, const MinMax &b) { return a.mn != b.mn || a.mx != b.mx; }

/// Test position.
inline bool Contains(const MinMax &mm, const Vector3 &p) { return !(p.x < mm.mn.x || p.y < mm.mn.y || p.z < mm.mn.z || p.x > mm.mx.x || p.y > mm.mx.y || p.z > mm.mx.z); }

/// Return the union of two minmax.
inline MinMax Union(const MinMax &a, const MinMax &b) { return MinMax(Vector3::Minimum(a.mn, b.mn), Vector3::Maximum(a.mx, b.mx)); }
/// Return the union of a minmax and vector.
inline MinMax Union(const MinMax &mm, const Vector3 &p) { return MinMax(Vector3::Minimum(mm.mn, p), Vector3::Maximum(mm.mx, p)); }

class Matrix4;

/// Return a transformed copy of a minmax instance.
MinMax operator*(const MinMax &mm, const Matrix4 &m);

/// Intersect ray with this minmax.
bool IntersectRay(const MinMax &mm, const Vector3 &o, const Vector3 &d, float &tmin, float &tmax);
bool IntersectRay(const MinMax &mm, const Vector3 &o, const Vector3 &d);

/// Returns whether a line intersect with the MinMax.
bool ClassifyLine(const MinMax &mm, const Vector3 &p, const Vector3 &d, Vector3 &i, Vector3 *n = nullptr);
/// Returns whether a segment intersect with the MinMax.
bool ClassifySegment(const MinMax &mm, const Vector3 &p0, const Vector3 &p1, Vector3 &i, Vector3 *n = nullptr);

/// Set from position and size.
inline MinMax MinMaxFromPositionSize(const Vector3 &p, const Vector3 &s) { return MinMax(p - s * 0.5f, p + s * 0.5f); }

} // namespace hg
