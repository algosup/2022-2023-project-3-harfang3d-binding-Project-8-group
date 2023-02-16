// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

namespace hg {

enum GpuCaps {
	/**
		@name Hardware caps
		@{
	*/
	GpuMaxTextureSize,
	GpuMaxTextureUnit,

	GpuCanBlitRBO,
	GpuTextureTopLeftOrigin,

	GpuMaxAnisotropy
	/// @}
};

enum GpuNPOTSupport {
	GpuNPOTNone,
	GpuNPOTLimited, //< NPOT support with no mip-map and wrap as clamp to edge only.
	GpuNPOTFull
};

} // namespace hg
