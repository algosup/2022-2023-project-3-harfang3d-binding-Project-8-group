// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstdint>

namespace hg {

enum TextureUV : uint8_t {
	TextureUVClamp,
	TextureUVRepeat,
	TextureUVMirror,
	TextureUVCount
};

enum TextureFilter : uint8_t {
	TextureFilterNearest,
	TextureFilterLinear,
	TextureFilterTrilinear,
	TextureFilterAnisotropic,
	TextureFilterCount
};

struct TextureUnitConfig {
	TextureUnitConfig() : wrap_u(TextureUVClamp), wrap_v(TextureUVClamp), wrap_w(TextureUVClamp), min_filter(TextureFilterLinear), mag_filter(TextureFilterLinear) {}
	TextureUnitConfig(TextureUV wrap_u_, TextureUV wrap_v_, TextureFilter min_filter_, TextureFilter mag_filter_) : wrap_u(wrap_u_), wrap_v(wrap_v_), wrap_w(TextureUVClamp), min_filter(min_filter_), mag_filter(mag_filter_) {}

	inline bool operator==(const TextureUnitConfig &cfg) const {
		return wrap_u == cfg.wrap_u && wrap_v == cfg.wrap_v && wrap_w == cfg.wrap_w && min_filter == cfg.min_filter && mag_filter == cfg.mag_filter;
	}
	inline bool operator!=(const TextureUnitConfig &cfg) const { return !(*this == cfg); }

	TextureUV wrap_u : 2, wrap_v : 2, wrap_w : 2;
	TextureFilter min_filter : 4, mag_filter : 4;
};

extern TextureUnitConfig default_texture_unit_config;

} // namespace hg
