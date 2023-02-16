// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/rect.h"
#include <memory>
#include <string>
#include <vector>

namespace hg {

struct GlyphInfo {
	iRect rect;
	Vector2 advance;
};

struct AggPimpl;

/// Manage font objects
class FontEngine {
public:
	static const int storage_size = 32;

	FontEngine();
	~FontEngine();

	bool SetFont(const std::string &path, bool autohint = false);
	void SetSize(float size);

	GlyphInfo GetGlyphInfo(std::u32string::value_type codepoint);
	fRect GetTextRect(const std::u32string::value_type *codepoints, size_t count, float x, float y);
	fRect GetTextRect(const std::string &utf8, float x, float y);

	bool GetKerning(std::u32string::value_type first_codepoint, std::u32string::value_type second_codepoint, float &kerning_x, float &kerning_y);

	std::unique_ptr<AggPimpl> agg_pimpl;

private:
	struct Font {
		std::string path;
		std::vector<char> data;
	};

	std::vector<Font> fonts;

	float size{12.f};
};

} // namespace hg
