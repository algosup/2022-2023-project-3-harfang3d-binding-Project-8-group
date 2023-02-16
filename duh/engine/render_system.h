// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/font_engine.h"
#include "engine/raster_font_batch.h"
#include "engine/render_primitive.h"
#include "engine/render_stats.h"
#include "engine/renderable.h"
#include "engine/renderer.h"
#include "engine/resource_cache.h"
#include "engine/state_cache.h"
#include "engine/view_state.h"

namespace hg {

class LightSystem;
struct RasterFont;

enum RenderTechnique {
	TechniqueForward,
	TechniqueDeferred
};

class Geometry;
struct Material;

class RenderGeometry;
class SurfaceShader;

class Node;

/// Render system
class RenderSystem : public std::enable_shared_from_this<RenderSystem> {
public:
	struct Config {
		Config() : fx_downscale(2), max_anisotropy(1), msaa_sample(1), enable_shadow(true) {}

		uint16_t shadow_size{1024};

		uint32_t fx_downscale : 4; // max 16
		uint32_t max_anisotropy : 4; // max 16
		uint32_t msaa_sample : 4; //max 16

		bool enable_shadow : 1;
	};

	struct ShadowMapData {
		std::shared_ptr<RenderTarget> rt;
		std::shared_ptr<Texture> map;

		float pcf_radius{1.75f};
	};

	struct DirectVBO {
		std::shared_ptr<GpuBuffer> helper_idx, helper_vtx;
		std::shared_ptr<GpuBuffer> direct_idx, direct_vtx;

		std::shared_ptr<GpuBuffer> box_idx; // for deferred light volume drawing
	};

	struct Fog {
		float znear{0}, zfar{0};
		Color color{Color::Black};
	};

	struct Environment {
		bool clear_bg_color{true};
		bool clear_bg_depth{true};
		Color clear_color{Color::Black};
		Color ambient_color{Color::Black};
	};

	RenderSystem();
	~RenderSystem();

	RenderSystem(const RenderSystem &render_system) = delete;

	const std::shared_ptr<Renderer> &GetRenderer() const { return renderer; }

	bool Initialize(std::shared_ptr<Renderer> renderer, bool support_3d = true);
	bool IsInitialized() const { return is_initialized; }
	void Free();

	void CreateShadowMaps(uint16_t resolution = 1024);
	void FreeShadowMaps();

protected:
	iVector2 internal_resolution{0, 0};

	std::shared_ptr<RenderGeometry> HasOrMakeGeometry(const std::string &name, bool use_cache, bool &was_in_cache);
	std::shared_ptr<RenderMaterial> HasOrMakeMaterial(const std::string &name, bool use_cache, bool &was_in_cache);

public:
	/// Return the system internal resolution.
	iVector2 GetInternalResolution() const { return internal_resolution; }
	/// Set the system internal resolution.
	void SetInternalResolution(const iVector2 &resolution);

	/// Get the viewport to internal resolution ratio.
	Vector2 GetViewportToInternalResolutionRatio() const;

	const RenderStatistics &GetStatistics() const { return stats; }

	const Config &GetConfiguration() const { return config; }
	void SetAA(uint32_t sample_count);

	const DirectVBO &GetDirectVBO() const { return direct_vbo; }
	const ShadowMapData &GetShadowMapData() const { return shadow_map_data; }

	size_t PurgeCache();
	void ClearCache();
	void RefreshCacheEntry(const std::string &name);

	bool HasSurfaceShader(const std::string &name) const;
	std::shared_ptr<SurfaceShader> GetSurfaceShader(const std::string &name) const;
	std::shared_ptr<SurfaceShader> LoadSurfaceShader(const std::string &name, const std::string &source, bool use_cache = true);
	std::shared_ptr<SurfaceShader> LoadSurfaceShader(const std::string &path, bool use_cache = true);

	bool HasMaterial(const std::string &name) const;
	std::shared_ptr<RenderMaterial> GetMaterial(const std::string &name) const;
	std::shared_ptr<RenderMaterial> LoadMaterial(const std::string &name, bool use_cache = true);
	std::shared_ptr<RenderMaterial> LoadMaterial(const std::string &name, const std::string &source, DocumentFormat format = DocumentFormatUnknown, bool use_cache = true);
	std::shared_ptr<RenderMaterial> CreateMaterial(const std::shared_ptr<Material> &core_mat, bool use_cache = true);

	bool HasGeometry(const std::string &name) const;
	std::shared_ptr<RenderGeometry> GetGeometry(const std::string &name) const;
	std::shared_ptr<RenderGeometry> LoadGeometry(const std::string &name, bool use_cache = true);
	std::shared_ptr<RenderGeometry> LoadGeometry(const std::string &name, const std::string &source, DocumentFormat format = DocumentFormatUnknown, bool use_cache = true);
	std::shared_ptr<RenderGeometry> CreateGeometry(const std::shared_ptr<Geometry> &core_geo, bool use_cache = true);

	const ResourceCache<RenderGeometry> &GetGeometryCache() const { return geometries; }
	const ResourceCache<RenderMaterial> &GetMaterialCache() const { return materials; }
	const ResourceCache<SurfaceShader> &GetSurfaceShaderCache() const { return surface_shaders; }

	RenderTechnique GetRenderTechnique() const { return render_technique; }
	void SetRenderTechnique(RenderTechnique technique, bool render_to_float_buffers = false);
	void FreeRenderTechnique();

	time_ns GetFrameClock() const { return frame_clock; }

	void OnContextChanged();

	void SetView(const Matrix4 &view, const Matrix44 &projection, Eye eye = EyeMono);
	void SetView(const Matrix4 &view, const Matrix4 &prv_view, const Matrix44 &projection, Eye eye = EyeMono);
	Eye GetEye() const { return eye; }

	ViewState GetViewState() const;
	void SetViewState(const ViewState &state);

	const FrustumPlanes &GetViewFrustum() const { return frustum; }

	uint32_t UpdateLightVolumeBuffers(const Node &light, GpuBuffer &idx, GpuBuffer &vtx);

	FontEngine font_engine;
	RasterFontBatch font_batch;

	void DrawRasterFontBatch() { font_batch.Draw(*this); }

	RenderStateCache state_cache;

	std::shared_ptr<GpuBuffer> instance_data_buffer;

	Fog fog;
	Environment environment;

	bool continuous_normals{true};

	std::shared_ptr<RenderTarget> render_fbo;

	std::shared_ptr<RenderTarget> buffer_fbo;
	std::shared_ptr<Texture> t_compose[2], t_depth; ///< 1:1
	std::shared_ptr<Texture> t_color_aa, t_depth_aa; ///< 1:1

	std::shared_ptr<RenderTarget> resolve_fbo; ///< For MSAA support.

	std::shared_ptr<RenderTarget> color_depth_read_fbo;
	std::shared_ptr<Texture> t_color_read, t_depth_read;

	std::shared_ptr<RenderTarget> fx_fbo;
	std::shared_ptr<Texture> t_fx[3], t_fx_depth;
	std::shared_ptr<Texture> t_gbuffer[4];

	std::shared_ptr<GpuShader> single_texture_2d_shader, single_texture_color_shader, single_texture_shader, single_color_shader, simple_shader, ambient_shader,
		pointlight_shader, pointlight_shadow_shader, spotlight_shader, spotlight_shadow_shader, linearlight_shader, linearlight_shadow_shader, ds_fog_shader;

	//
	void SetSurfaceShaderInt(SurfaceShader &shader, const std::string &name, const int *v, size_t count = 1);
	void SetSurfaceShaderInt2(SurfaceShader &shader, const std::string &name, const int *vec2, size_t count = 1);
	void SetSurfaceShaderInt3(SurfaceShader &shader, const std::string &name, const int *vec3, size_t count = 1);
	void SetSurfaceShaderInt4(SurfaceShader &shader, const std::string &name, const int *vec4, size_t count = 1);
	void SetSurfaceShaderUnsigned(SurfaceShader &shader, const std::string &name, const uint32_t *v, size_t count = 1);
	void SetSurfaceShaderUnsigned2(SurfaceShader &shader, const std::string &name, const uint32_t *vec2, size_t count = 1);
	void SetSurfaceShaderUnsigned3(SurfaceShader &shader, const std::string &name, const uint32_t *vec3, size_t count = 1);
	void SetSurfaceShaderUnsigned4(SurfaceShader &shader, const std::string &name, const uint32_t *vec4, size_t count = 1);
	void SetSurfaceShaderFloat(SurfaceShader &shader, const std::string &name, const float *v, size_t count = 1);
	void SetSurfaceShaderFloat2(SurfaceShader &shader, const std::string &name, const float *vec2, size_t count = 1);
	void SetSurfaceShaderFloat3(SurfaceShader &shader, const std::string &name, const float *vec3, size_t count = 1);
	void SetSurfaceShaderFloat4(SurfaceShader &shader, const std::string &name, const float *vec4, size_t count = 1);
	void SetSurfaceShaderMatrix3(SurfaceShader &shader, const std::string &name, const Matrix3 *mtx3, size_t count = 1);
	void SetSurfaceShaderMatrix4(SurfaceShader &shader, const std::string &name, const Matrix4 *mtx4, size_t count = 1);
	void SetSurfaceShaderMatrix44(SurfaceShader &shader, const std::string &name, const Matrix44 *mtx44, size_t count = 1);
	void SetSurfaceShaderTexture(SurfaceShader &shader, const std::string &name, const std::shared_ptr<Texture> &tex);

	//
	Signal<void(RenderSystem &)> begin_draw_frame_signal;
	Signal<bool(RenderSystem &)> set_render_target_signal;
	Signal<bool(RenderSystem &)> clear_frame_signal;
	Signal<void(RenderSystem &)> frame_cleared_signal;
	Signal<void(RenderSystem &)> renderables_drawn_signal;
	Signal<void(RenderSystem &, RenderPass::Pass)> end_render_pass_signal;
	Signal<void(RenderSystem &)> frame_complete_signal;

	Signal<bool(RenderSystem &, const RenderPrimitiveLists &, const std::shared_ptr<RenderTarget> &, const std::shared_ptr<Texture> &, const std::shared_ptr<Texture> &)> apply_post_processes_signal;

	/**
		@name Low-level draw
		@{
	*/
	/// Select and configure a built-in shader for a simple draw operation.
	bool SetShaderAuto(bool has_color);
	bool SetShaderAuto(bool has_color, const Texture &);

	/// Draw a set of lines.
	void DrawLine(size_t count, const Vector3 *pos, const Color *col = nullptr, const Vector2 *uv = nullptr, const uint16_t *idx = nullptr, size_t vtx_count = 0);
	/// Draw a set of triangles.
	void DrawTriangle(size_t count, const Vector3 *pos, const Color *col = nullptr, const Vector2 *uv = nullptr, const uint16_t *idx = nullptr, size_t vtx_count = 0);
	/// Draw a set of sprites.
	void DrawSprite(size_t count, const Vector3 *pos, const Color *col = nullptr, const float *size = nullptr, float global_size = 1.f);

	/// Draw a set of lines using a built-in shader selected by the render system.
	void DrawLineAuto(size_t count, const Vector3 *pos, const Color *col = nullptr, const Vector2 *uv = nullptr, const Texture * = nullptr, const uint16_t *idx = nullptr, size_t vtx_count = 0);
	/// Draw a set of triangles using a built-in shader selected by the render system.
	void DrawTriangleAuto(size_t count, const Vector3 *pos, const Color *col = nullptr, const Vector2 *uv = nullptr, const Texture * = nullptr, const uint16_t *idx = nullptr, size_t vtx_count = 0);
	/// Draw a set of sprites using a built-in shader selected by the render system.
	void DrawSpriteAuto(size_t count, const Vector3 *pos, const Texture &tex, const Color *col = nullptr, const float *size = nullptr, float global_size = 1.f);

	/// Draw a quad in screen space.
	void DrawQuad2D(const fRect &src, const fRect &dst);
	/// Draw a fullscreen quad.
	void DrawFullscreenQuad(const Vector2 &uv);
	/// @}

	/// Draw a geometry using the current shader ignoring its materials and surface shaders.
	void DrawGeometrySimple(const RenderGeometry &geo);
	/// Draw a scene using the current shader ignoring its materials and surface shaders.
	void DrawSceneSimple(const Scene &scene);

	bool BeginDrawFrame();
	void DrawRenderables(const std::vector<std::shared_ptr<Renderable>> &renderables, LightSystem &light_system);
	void DrawRenderPassShader(const std::vector<std::shared_ptr<Renderable>> &renderables, RenderPass::Shader shader);
	void EndDrawFrame();
	void ResolveFrame();

	void ApplyPostProcesses(const std::vector<std::shared_ptr<Component>> &post_process_components);

	const std::shared_ptr<Texture> &GetShadowMap() const { return shadow_map_data.map; }

	/// Return the default raster font object.
	const std::shared_ptr<RasterFont> &GetDefaultFont() const { return default_font; }

private:
	RenderPrimitiveLists prim_lists;

	friend void DrawRenderPrimitives(RenderSystem &render_system, const RenderPrimitive *prims, size_t count, const RenderPass &pass);

	std::shared_ptr<Renderer> renderer;

	std::atomic<bool> is_initialized{false};

protected:
	friend class PendingRenderTaskGuard;
	std::atomic<uint32_t> pending_tasks{0};

private:
	Config config;
	RenderStatistics stats;

	ResourceCache<RenderMaterial> materials;
	ResourceCache<RenderGeometry> geometries;
	ResourceCache<SurfaceShader> surface_shaders;

	RenderTechnique render_technique{TechniqueForward};

	/// Configure the render system according to the underlying renderer caps.
	void DiscoverConfiguration();

	void LoadCoreShaders(bool support_3d = true);
	void FreeCoreShaders();

	time_ns frame_clock{0};

	VertexLayout simple_vertex_layout;

	DirectVBO direct_vbo;
	ShadowMapData shadow_map_data;

	std::shared_ptr<RenderTarget> render_target;
	std::shared_ptr<Texture> t_final;

	Eye eye;
	FrustumPlanes frustum;

	std::shared_ptr<RasterFont> default_font;
};

//
class PendingRenderTaskGuard {
public:
	PendingRenderTaskGuard(RenderSystem &render_system) : system(render_system) { ++system.pending_tasks; }
	~PendingRenderTaskGuard() { --system.pending_tasks; }

private:
	RenderSystem &system;
};

/// Given a list of renderable, extract all the render primitives in a given context.
void BuildFrustumPrimitiveLists(const std::vector<std::shared_ptr<Renderable>> &renderables, const FrustumPlanes &frustum, const Matrix4 &frustum_matrix, const FrustumPlanes &view_frustum, const Matrix4 &view_matrix, RenderPrimitiveLists &primitive_lists, RenderableContext ctx = ContextDefault);
/// Call the render system end of pass hooks.
void EmitEndRenderPassSignal(RenderSystem &render_system, RenderPass::Pass pass);

extern RenderSystem *g_render_system;

} // namespace hg
