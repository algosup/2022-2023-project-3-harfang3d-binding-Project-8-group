// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/unit.h"
#include "foundation/vector2.h"

namespace hg {

struct Color;
struct Vector3;
class Matrix3;
class Matrix4;
struct OBB;
struct MinMax;
struct Texture;
class Geometry;
class RenderSystem;

/// Wrapper to display a cross in 3D.
void DrawCross(RenderSystem &, const Vector3 &, float size = Mtr(1), const Color * = nullptr);
/// Wrapper to display an axis aligned square in 3D.
void DrawSquare(RenderSystem &, const Vector3 &, float size = Mtr(1));
/// Wrapper to display an OOBB using the line function of a derived renderer.
void DrawOBB(RenderSystem &, const OBB &, const Color * = nullptr);
/// Wrapper to display an OOBB using the triangle function of a derived renderer.
void DrawFilledOBB(RenderSystem &, const OBB &, const Color *);
/// Wrapper to display an AABB in 3D.
void DrawAABB(RenderSystem &, const MinMax &, const Color * = nullptr);
/// Wrapper to display a cube in 3D.
void DrawCube(RenderSystem &, const Vector3 &, float size = Mtr(1), const Color * = nullptr);
/// Wrapper to display a sphere in 3D.
void DrawSphere(RenderSystem &, const Vector3 &, float radius = Mtr(1), const Color * = nullptr);
/// Wrapper to display a ball in 3D.
void DrawBall(RenderSystem &, const Matrix4 &, float radius = Mtr(1), Color * = nullptr);
/// Wrapper to display a circle in 3D.
void DrawCircle(RenderSystem &, const Vector3 &, float radius = Mtr(1), const Matrix3 * = nullptr, const Color * = nullptr);
/// Wrapper to display a cylinder in 3D.
void DrawCylinder(RenderSystem &, const Vector3 &, float radius = Mtr(1), float length = Mtr(1), const Matrix3 * = nullptr, const Color * = nullptr);
/// Wrapper to display a capsule in 3D.
void DrawCapsule(RenderSystem &, const Vector3 &, float radius = Mtr(1.f), float length = Mtr(1), const Matrix3 * = nullptr, const Color * = nullptr);
/// Wrapper to display a cone in 3D.
void DrawCone(RenderSystem &, const Vector3 &, float radius = Mtr(1.f), float length = Mtr(1), const Matrix3 * = nullptr, const Color * = nullptr);
/// Draw Geometry as wireframe.
void DrawGeometry(RenderSystem &, Geometry &, const Matrix4 &, Color &, float scale = 1.f);

/// Wrapper to line 3D.
void Line3D(RenderSystem &, const Vector3 &, const Vector3 &, const Color * = nullptr);
/// Draw triangle.
void Triangle3D(RenderSystem &, const Vector3[3], const Color[3] = nullptr, const Vector2[3] = nullptr, const Texture * = nullptr);

/// Return nearest but highest power of two from a given value.
template <class T>
T GetPow2(T v, bool no_shrink = false) {
	T h = 1;
	if (no_shrink)
		while (h < v)
			h *= 2;
	else
		while ((h + h / 2) < v)
			h *= 2;
	return h;
}

} // namespace hg
