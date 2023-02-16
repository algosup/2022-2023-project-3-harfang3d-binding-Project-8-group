// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/math.h"
#include "foundation/rect.h"
#include "foundation/reflection.h"
#include "foundation/vector2.h"
#include "foundation/declare_type_name.h"
#include <vector>

namespace hg {

class Matrix44;
class Matrix4;
class Matrix3;
struct Quaternion;
struct Vector3;
struct Vector4;
struct MinMax;
struct Color;
struct OBB;

void RegisterFrameworkReflection(TypeRegistry &registry);

DeclareTypeName(MinMax, "MinMax");
DeclareTypeName(OBB, "OBB");

DeclareTypeName(RotationOrder, "RotationOrder");
DeclareTypeName(Axis, "Axis");

DeclareTypeName(BinaryData, "BinaryData");

DeclareTypeName(Color, "Color");
DeclareTypeName(std::vector<Color>, "ColorVector");
DeclareTypeName(Vector2, "Vector2");
DeclareTypeName(std::vector<Vector2>, "Vector2Vector");
DeclareTypeName(iVector2, "IntVector2");
DeclareTypeName(std::vector<iVector2>, "IntVector2Vector");
DeclareTypeName(Vector3, "Vector3");
DeclareTypeName(std::vector<Vector3>, "Vector3Vector");
DeclareTypeName(Vector4, "Vector4");
DeclareTypeName(std::vector<Vector4>, "Vector4Vector");
DeclareTypeName(Quaternion, "Quaternion");
DeclareTypeName(std::vector<Quaternion>, "QuaternionVector");
DeclareTypeName(Matrix3, "Matrix3");
DeclareTypeName(std::vector<Matrix3>, "Matrix3Vector");
DeclareTypeName(Matrix4, "Matrix4");
DeclareTypeName(std::vector<Matrix4>, "Matrix4Vector");
DeclareTypeName(Matrix44, "Matrix44");
DeclareTypeName(std::vector<Matrix44>, "Matrix44Vector");
DeclareTypeName(fRect, "FloatRect");
DeclareTypeName(std::vector<fRect>, "FloatRectVector");
DeclareTypeName(iRect, "IntRect");
DeclareTypeName(std::vector<iRect>, "IntRectVector");

} // namespace hg
