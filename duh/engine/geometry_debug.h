// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

namespace hg {

struct Color;
class RenderSystem;

class Geometry;

void DrawGeometryNormals(RenderSystem &sys, const Geometry &geo, const Color &N_color, float size = 0.1f);
void DrawGeometryTangents(RenderSystem &sys, const Geometry &geo, const Color &T_color, const Color &B_color, float size = 0.1f);

} // namespace hg
