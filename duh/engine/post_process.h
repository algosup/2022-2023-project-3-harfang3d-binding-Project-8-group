// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "foundation/reflection.h"
#include "foundation/vector2.h"

namespace hg {

class Renderer;
class RenderSystem;
struct RenderTarget;
struct GpuShader;
struct RenderPrimitiveLists;
struct Texture;

/// Post-process component interface
struct PostProcessComponent : Component {
	/**
		@short Apply post-process to the input textures.
		@return True to swap the frame ping-pong texture set, false if the
		post-process is done directly over the input.
	*/
	virtual bool Apply(RenderSystem &rs, const RenderPrimitiveLists &prim_lists, const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &t_in, const std::shared_ptr<Texture> &t_out) = 0;
};

/// Sharpen post-process
class SharpenPostProcess : public std::enable_shared_from_this<SharpenPostProcess>, public PostProcessComponent {
public:
	SharpenPostProcess();

	decl_component_defaults(SharpenPostProcess, "SharpenPostProcess", "PostProcess");
	comp_decl_member_get_sync_set_async(SharpenPostProcess, float, Strength, strength);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	bool Apply(RenderSystem &rs, const RenderPrimitiveLists &prim_lists, const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &t_in, const std::shared_ptr<Texture> &t_out) override;

private:
	std::shared_ptr<GpuShader> shader;
};

/// Radial blur post-process
class RadialBlurPostProcess : public std::enable_shared_from_this<RadialBlurPostProcess>, public PostProcessComponent {
public:
	RadialBlurPostProcess();

	decl_component_defaults(RadialBlurPostProcess, "RadialBlurPostProcess", "PostProcess");

	comp_decl_member_get_sync_set_async_validated(RadialBlurPostProcess, float, Strength, strength, ClampToZero);
	comp_decl_member_get_sync_set_async(RadialBlurPostProcess, Vector2, Center, center);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	bool Apply(RenderSystem &rs, const RenderPrimitiveLists &prim_lists, const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &t_in, const std::shared_ptr<Texture> &t_out) override;

private:
	std::shared_ptr<GpuShader> shader;
};

/// Chromatic dispersion post-process
class ChromaticDispersionPostProcess : public std::enable_shared_from_this<ChromaticDispersionPostProcess>, public PostProcessComponent {
public:
	ChromaticDispersionPostProcess();

	decl_component_defaults(ChromaticDispersionPostProcess, "ChromaticDispersionPostProcess", "PostProcess");
	comp_decl_member_get_sync_set_async(ChromaticDispersionPostProcess, float, Width, width);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	bool Apply(RenderSystem &rs, const RenderPrimitiveLists &prim_lists, const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &t_in, const std::shared_ptr<Texture> &t_out) override;

private:
	std::shared_ptr<GpuShader> shader;
};

/// Hue/Saturation/Luminance post-process
class HSLPostProcess : public std::enable_shared_from_this<HSLPostProcess>, public PostProcessComponent {
public:
	HSLPostProcess();

	decl_component_defaults(HSLPostProcess, "HLSLPostProcess", "PostProcess");
	comp_decl_member_get_sync_set_async(HSLPostProcess, float, H, H);
	comp_decl_member_get_sync_set_async(HSLPostProcess, float, S, S);
	comp_decl_member_get_sync_set_async(HSLPostProcess, float, L, L);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	bool Apply(RenderSystem &rs, const RenderPrimitiveLists &prim_lists, const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &t_in, const std::shared_ptr<Texture> &t_out) override;

private:
	std::shared_ptr<GpuShader> shader;
};

/// Motion blur post-process
class MotionBlurPostProcess : public std::enable_shared_from_this<MotionBlurPostProcess>, public PostProcessComponent {
public:
	MotionBlurPostProcess();

	decl_component_defaults(MotionBlurPostProcess, "MotionBlurPostProcess", "PostProcess");

	comp_decl_member_get_sync_set_async(MotionBlurPostProcess, int, BlurRadius, blur_radius);
	comp_decl_member_get_sync_set_async(MotionBlurPostProcess, int, SampleCount, sample_count);
	comp_decl_member_get_sync_set_async(MotionBlurPostProcess, float, Exposure, exposure);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	bool Apply(RenderSystem &rs, const RenderPrimitiveLists &prim_lists, const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &t_in, const std::shared_ptr<Texture> &t_out) override;

private:
	bool LoadShaders(const std::shared_ptr<hg::Renderer> &renderer);
	bool UpdateTextures(const std::shared_ptr<Renderer> &renderer, uint16_t width, uint16_t height);

private:
	std::shared_ptr<GpuShader> tile_max, neighbour_max, reconstruction;
	std::shared_ptr<Texture> depth_map, normal_depth_map, velocity_map, tile_max_map, neighbour_max_map, random_map;
};

class SAOPostProcess : public std::enable_shared_from_this<SAOPostProcess>, public PostProcessComponent {
public:
	SAOPostProcess();

	decl_component_defaults(SAOPostProcess, "SAOPostProcess", "PostProcess");
	comp_decl_member_get_sync_set_async(SSAOPostProcess, float, Bias, bias);
	comp_decl_member_get_sync_set_async(SSAOPostProcess, float, Radius, radius);
	comp_decl_member_get_sync_set_async(SSAOPostProcess, int, Quality, quality);
	comp_decl_member_get_sync_set_async(SSAOPostProcess, int, SampleCount, sample_count);
	comp_decl_member_get_sync_set_async(SSAOPostProcess, float, Sharpness, sharpness);
	comp_decl_member_get_sync_set_async(SSAOPostProcess, float, Strength, strength);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	bool Apply(RenderSystem &rs, const RenderPrimitiveLists &prim_lists, const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &t_in, const std::shared_ptr<Texture> &t_out) override;

private:
	bool UpdateTextures(const std::shared_ptr<Renderer> &renderer, uint16_t width, uint16_t height);
	bool LoadShaders(const std::shared_ptr<Renderer> &renderer);
private:
	std::shared_ptr<GpuShader> downsample, compute, upsample, blur, combine;
	std::shared_ptr<Texture> depth_buffer;
	std::shared_ptr<Texture> normal_depth_map[2];
	std::shared_ptr<Texture> ao_map[3];
};

class BloomPostProcess : public std::enable_shared_from_this<BloomPostProcess>, public PostProcessComponent {
public:
	BloomPostProcess();

	decl_component_defaults(BloomPostProcess, "BloomPostProcess", "PostProcess");
	comp_decl_member_get_sync_set_async(BloomPostProcess, float, Threshold, threshold);
	comp_decl_member_get_sync_set_async(BloomPostProcess, float, Smoothness, smoothness);
	comp_decl_member_get_sync_set_async(BloomPostProcess, float, Blur, blur);
	comp_decl_member_get_sync_set_async(BloomPostProcess, float, Radius, radius);
	comp_decl_member_get_sync_set_async(BloomPostProcess, float, Intensity, intensity);
	comp_decl_member_get_sync_set_async(BloomPostProcess, int, Quality, quality);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	bool Apply(RenderSystem &render_system, const RenderPrimitiveLists &prim_lists, const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &t_in, const std::shared_ptr<Texture> &t_out) override;

private:
	bool UpdateTextures(const std::shared_ptr<Renderer> &renderer, uint16_t width, uint16_t height);
	bool LoadShaders(const std::shared_ptr<Renderer> &renderer);
private:
	std::shared_ptr<GpuShader> hi_pass[2], downsample[2], upsample[2], combine[2];
	std::vector<std::shared_ptr<Texture>> pyramid[2];
};

class ComponentFactory;

void RegisterCorePostProcessComponents(ComponentFactory &factory);

} // namespace hg
