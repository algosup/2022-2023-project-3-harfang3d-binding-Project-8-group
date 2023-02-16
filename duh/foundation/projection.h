// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/matrix44.h"
#include "foundation/vector2.h"

namespace hg {

float ZoomFactorToFov(float zoom_factor);
float FovToZoomFactor(float fov);

Matrix44 ComputeOrthographicProjectionMatrix(float znear, float zfar, float size, const Vector2 &aspect_ratio);
Matrix44 ComputePerspectiveProjectionMatrix(float znear, float zfar, float zoom_factor, const Vector2 &aspect_ratio);
Matrix44 Compute2DProjectionMatrix(float znear, float zfar, float res_x, float res_y, bool y_up);

/// Extract zoom factor from a projection matrix.
float ExtractZoomFactorFromProjectionMatrix(const Matrix44 &m);
/// Extract Z near and Z far from a projection matrix.
void ExtractZRangeFromProjectionMatrix(const Matrix44 &m, float &znear, float &zfar);

} // namespace hg
