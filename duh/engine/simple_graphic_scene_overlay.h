// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/scene_overlay.h"
#include "engine/simple_graphic_engine.h"
#include <string>

namespace hg {

class SimpleGraphicSceneOverlay : public std::enable_shared_from_this<SimpleGraphicSceneOverlay>, public SceneOverlay {
public:
	SimpleGraphicSceneOverlay(bool is_2d_overlay = true);

	decl_component_defaults(SimpleGraphicSceneOverlay, "SceneOverlay", "SceneOverlay");

	comp_decl_member_get_sync_set_async(SimpleGraphicSceneOverlay, bool, Is2DOverlay, is_2d_overlay);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	void SetSnapGlyphToGrid(bool v);
	bool GetSnapGlyphToGrid() const;

	void SetBlendMode(BlendMode mode);
	BlendMode GetBlendMode() const;
	void SetCullMode(CullMode mode);
	CullMode GetCullMode() const;

	void SetDepthWrite(bool write);
	bool GetDepthWrite() const;
	void SetDepthTest(bool test);
	bool GetDepthTest() const;

	void Line(float sx, float sy, float sz, float ex, float ey, float ez, const Color &start_color, const Color &end_color);
	void Triangle(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, const Color &a_color, const Color &b_color, const Color &c_color);
	void Text(float x, float y, float z, const std::string &text, const Color &color, const std::shared_ptr<RasterFont> &font, float scale = 1.f);
	void Text(const Matrix4 &mat, const std::string &text, const Color &color, const std::shared_ptr<RasterFont> &font, float scale = 1.f);
	void Quad(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, float dx, float dy, float dz, float uv_sx, float uv_sy, float uv_ex, float uv_ey, const std::shared_ptr<Texture> &tex, const Color &a_color, const Color &b_color, const Color &c_color, const Color &d_color);
	void Geometry(float x, float y, float z, float ex, float ey, float ez, float sx, float sy, float sz, const std::shared_ptr<RenderGeometry> &geo);

	/// Draw the render overlay, this method is called from the render thread.
	void Draw(RenderSystem &render_system) override;
	/// Called after the scene complete.
	void Clear(RenderSystem &render_system) override;

private:
	SimpleGraphicEngine graphic_engine;
};

} // namespace hg
