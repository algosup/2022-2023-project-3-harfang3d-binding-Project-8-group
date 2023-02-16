// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/picture.h"
#include "foundation/color.h"
#include "foundation/rect_packer.h"
#include "foundation/utf8.h"
#include <map>

namespace hg {

struct Texture;

class RenderSystem;

/**
	@short Implement a raster font rasterizer and renderer.
	@note Must only be called from the render system thread.
*/
struct RasterFont {
	RasterFont(const std::string &font_path, float font_size, uint32_t page_resolution = 512, uint32_t glyph_margin = 2, bool autohint = false);

	/**
		@short Prepare the glyph pages required to render a given text.
		This function can be used to prepare glyph pages before actual text
		drawing. Calling it before Draw() is optional.
	*/
	bool Prepare(RenderSystem &render_system, const std::string &utf8);
	/// Write an UTF8 string.
	bool Write(RenderSystem &render_system, const std::string &utf8, const Matrix4 &mat, Color color = Color::White, float scale = 1.f, bool snap_glyph_to_grid = true, bool orient_toward_camera = true);

	/// Return an UTF8 string bounding rectangle.
	fRect GetTextRect(RenderSystem &render_system, const std::string &utf8);

	bool Load(const RenderSystem &render_system, const std::string &base_path);
	bool Save(const RenderSystem &render_system, const std::string &base_path) const;

	float GetSize() const { return size; }

private:
	std::string path;
	float size;
	bool hinting;

	uint32_t margin, resolution;

	Picture scratchpad; // page creation scratchpad texture

	struct Page {
		std::shared_ptr<Texture> texture;
		RectPacker<int> packer;
	};

	std::vector<Page> pages;

	struct Glyph {
		int page{-1};
		GlyphInfo info;
		fRect uv;
	};

	std::map<utf32_cp, Glyph> glyphs;

	void PrepareGlyphs(RenderSystem &render_system, const utf32_cp *cps, size_t count);

	// kerning cache
	struct GlyphPair {
		GlyphPair(utf32_cp a_, utf32_cp b_) : a(a_), b(b_) {}
		bool operator<(const GlyphPair &p) const { return a < p.a && b < p.b; }

		utf32_cp a, b;
	};

	std::map<GlyphPair, float> kerning_cache;

	float GetKerning(FontEngine &engine, utf32_cp p, utf32_cp c);
};

} // namespace hg
