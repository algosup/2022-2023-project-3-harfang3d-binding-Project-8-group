// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#include "foundation/frustum.h"

namespace hg {

class RenderSystem;

void DrawFrustum(RenderSystem &render_system, const Frustum &frustum, Color color);
void DrawMinMax(RenderSystem &render_system, const size_t count, const MinMax *mm, Color color);
void DrawCross(RenderSystem &render_system, const size_t count, const Vector3 *pos, float size, Color color);
void DrawSphere(RenderSystem &render_system, const size_t count, const Vector3 *pos, const float *radius, Color color);
void DrawCapsule(RenderSystem &render_system, const size_t count, const Vector3 *pos, const float *radius, const float *half_length, Color color);

} // namespace hg
