// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/gpu_resource.h"
#include "foundation/rect.h"
#include "foundation/task.h"

namespace hg {

enum TextureUsage {
	TextureIsRenderTarget = 0x01,
	TextureIsShaderResource = 0x02,
	TextureUsageDefault = TextureIsShaderResource
};

enum TextureFormat : uint8_t {
	TextureRGBA8, ///< 4 channels 8 bit integer texture
	TextureBGRA8, ///< 4 channels 8 bit integer texture
	TextureRGBA16, ///< 4 channels 16 bit integer texture
	TextureRGBAF, ///< 4 channels float texture
	TextureDepth, ///< integer depth texture
	TextureDepthF, ///< float depth texture
	TextureR8, ///< single channel 8 bit integer texture
	TextureR16, ///< single channel 16 bit integer texture
	TextureRG8, ///< 2 channels 8 bit integer texture
	TextureInvalidFormat
};

enum TextureAA : uint8_t {
	TextureNoAA,
	TextureMSAA2x,
	TextureMSAA4x,
	TextureMSAA8x,
	TextureMSAA16x,
	TextureAALast
};

union TextureId {
	int32_t i32;
	int64_t i64;
	void *void_p{nullptr};
};

/// GPU Texture
struct Texture : public GpuResource {
	explicit Texture(const std::string &name = "Texture");
	~Texture() {}

	virtual uint16_t GetWidth() const = 0;
	virtual uint16_t GetHeight() const = 0;
	virtual uint8_t GetDepth() const = 0;

	virtual fRect GetRect() const;

	virtual TextureId GetNativeId() const = 0;
};

class Renderer;

/// Delete a texture on the renderer thread.
void TextureDeleter(Texture *texture, std::weak_ptr<Renderer> renderer_weak, task_affinity worker);

} // namespace hg
