// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/gpu_buffer.h"
#include "engine/gpu_caps.h"
#include "engine/gpu_shader.h"
#include "engine/gpu_shader_compiler.h"
#include "engine/picture.h"
#include "engine/render_target.h"
#include "engine/resource_cache.h"
#include "engine/vertex_layout.h"
#include "foundation/global_object.h"
#include "foundation/matrix4.h"
#include "foundation/matrix44.h"
#include "foundation/reflection.h"
#include "foundation/task_system.h"
#include "foundation/vector2.h"
#include "platform/window_system.h"

namespace hg {

/// GPU renderer matrices
struct RendererMatrices {
	Matrix4 view, iview, world, iworld;
	Matrix44 projection;
};

struct GpuHardwareInfo {
	std::string name, vendor;
};

struct GpuShaderVariable {
	uint32_t data[2]; // 8 bytes payload
};

enum TextureBackgroundBlit {
	TextureBackgroundBlitDefault = 0x0,
	TextureBackgroundBlitNoDataCopy = 0x1 ///< Do not make a copy of the CPU data (it must be available as long as the blit operation has not completed).
};

/// GPU based renderer class
class Renderer : public std::enable_shared_from_this<Renderer> {
public:
	Renderer();
	virtual ~Renderer();

	/// Returns the renderer's name as a C string.
	virtual const char *GetName() const = 0;
	/// Returns a C string describing the renderer.
	virtual const char *GetDescription() const = 0;
	/// Returns a C string containing the renderer version number.
	virtual const char *GetVersion() const = 0;
	/// Return a native handle to the rendering API.
	virtual void *GetNativeHandle() const = 0;

protected:
	GpuHardwareInfo hw_info;

	ResourceCache<Texture> textures;
	ResourceCache<GpuShader> shaders;

public:
	const GpuHardwareInfo &GetHardwareInfo() const { return hw_info; }

	size_t PurgeCache();
	void ClearCache();
	void RefreshCacheEntry(const std::string &name);

	virtual bool IsCooked(const std::string &name) const;

	const ResourceCache<Texture> &GetTextureCache() const { return textures; }
	const ResourceCache<GpuShader> &GetShaderCache() const { return shaders; }

	Signal<void(Renderer &)> open_signal, close_signal;
	Signal<void(Renderer &, const Window &, const Surface &)> output_window_surface_created_signal;
	Signal<void(Renderer &, const Surface &)> output_surface_created_signal;
	Signal<void(Renderer &, const Surface &)> output_surface_changed_signal;
	Signal<void(Renderer &, const Surface &)> output_surface_destroyed_signal;

	Signal<void(Renderer &)> pre_draw_frame_signal, post_draw_frame_signal;
	Signal<void(Renderer &)> show_frame_signal;

	/**
		@name Asynchronous interface
		@{
	*/
	/// Assert that the caller thread is the rendering thread.
	bool AssertCallerThreadCorrectness() const;
	/// @}

	/**
		@name Render target
		@{
	*/
protected:
	std::shared_ptr<RenderTarget> render_target;

public:
	virtual std::shared_ptr<RenderTarget> NewRenderTarget() = 0;

	virtual void SetRenderTargetColorTexture(RenderTarget &rt, std::shared_ptr<Texture> t);
	virtual void SetRenderTargetColorTexture(RenderTarget &rt, std::shared_ptr<Texture> *t, uint32_t count);
	virtual void SetRenderTargetDepthTexture(RenderTarget &rt, std::shared_ptr<Texture> t);

	/// Blit attachment to another FBO attachment.
	virtual void BlitRenderTarget(const std::shared_ptr<RenderTarget> &src_rt, const std::shared_ptr<RenderTarget> &dst_rt, const iRect &src, const iRect &dst, bool color = true, bool depth = true) = 0;
	/// Transfer color pixels into a CPU based buffer, this buffer is expected to be big enough to hold the region in RGBA format.
	virtual void ReadRenderTargetColorPixels(const std::shared_ptr<RenderTarget> &rt, void *out, const iRect &rect) = 0;

	/// Get current render target.
	const std::shared_ptr<RenderTarget> &GetRenderTarget() const { return render_target; }
	/// Clear current render target (render to back buffer).
	virtual void ClearRenderTarget() = 0;
	/// Set current render target.
	virtual void SetRenderTarget(std::shared_ptr<RenderTarget> rt) = 0;
	/// Check the current render target state.
	virtual bool CheckRenderTarget() = 0;

	virtual bool CreateRenderTarget(RenderTarget &rt) = 0;
	virtual void FreeRenderTarget(RenderTarget &rt) = 0;
	/// @}

	/**
		@name Buffer
		@{
	*/
	virtual std::shared_ptr<GpuBuffer> NewBuffer() = 0;
	std::shared_ptr<GpuBuffer> NewBuffer(size_t size, GpuBufferType type, GpuBufferUsage usage = GpuBufferStatic);
	std::shared_ptr<GpuBuffer> NewBuffer(const void *data, size_t size, GpuBufferType type, GpuBufferUsage usage = GpuBufferStatic);

	virtual size_t GetBufferSize(GpuBuffer &buffer) = 0;

	virtual void *MapBuffer(GpuBuffer &buffer) = 0;
	virtual void UnmapBuffer(GpuBuffer &buffer) = 0;

	virtual bool UpdateBuffer(GpuBuffer &buffer, const void *data, size_t start = 0, size_t size = 0) = 0;

	virtual bool CreateBuffer(GpuBuffer &buffer, const void *data, size_t size, GpuBufferType type, GpuBufferUsage usage = GpuBufferStatic) = 0;
	virtual void FreeBuffer(GpuBuffer &buffer) = 0;
	/// @}

	/**
		@name Texture
		@{
	*/
	virtual std::shared_ptr<Texture> NewTexture(const std::string &name = "Texture") = 0;
	virtual std::shared_ptr<Texture> NewShadowMap(int width, int height, const std::string &name = "Shadow") = 0;
	virtual std::shared_ptr<Texture> NewExternalTexture(const std::string &name = "External") = 0; // external texture are used to stream media files on mobile devices

	bool CreateTexture(Texture &tex, int width, int height, TextureFormat format = TextureRGBA8, TextureAA aa = TextureNoAA, TextureUsage type = TextureUsageDefault, bool mipmapped = true) { return CreateTexture(tex, nullptr, 0, width, height, format, aa, type, mipmapped); }
	virtual bool CreateTexture(Texture &tex, const void *data, size_t data_size, int width, int height, TextureFormat format = TextureRGBA8, TextureAA aa = TextureNoAA, TextureUsage type = TextureUsageDefault, bool mipmapped = true) = 0;
	bool CreateTexture(Texture &tex, const Picture &pic, TextureUsage usage = TextureUsageDefault, bool mipmapped = true);

	std::shared_ptr<Texture> NewTexture(const std::string &name, int width, int height, TextureFormat format = TextureRGBA8, TextureAA aa = TextureNoAA, TextureUsage usage = TextureUsageDefault, bool mipmapped = true);
	std::shared_ptr<Texture> NewTexture(const std::string &name, const void *data, size_t data_size, int width, int height, TextureFormat format = TextureRGBA8, TextureAA = TextureNoAA, TextureUsage usage = TextureUsageDefault, bool mipmapped = true);
	std::shared_ptr<Texture> NewTexture(const std::string &name, const Picture &pic, TextureUsage usage = TextureUsageDefault, bool mipmapped = true);

	virtual void FreeTexture(Texture &tex) = 0;

	virtual bool LoadNativeTexture(Texture &tex, const std::string &path) = 0;
	/// Return the texture file extensions supported natively by the renderer.
	virtual const char *GetNativeTextureExt() const = 0;

	/// Blit CPU data to texture.
	virtual void BlitTexture(Texture &tex, const void *data, size_t data_size, uint32_t width, uint32_t height, uint32_t x = 0, uint32_t y = 0) = 0;
	/// Resize texture, does not preserve content.
	virtual void ResizeTexture(Texture &tex, uint32_t width, uint32_t height) = 0;

	/**
		@short Blit CPU data to texture on a worker thread.

		The caller thread returns immediately and the texture resource is marked
		as not ready. When the blit operation completes, calling IsReady() on
		the texture resource will return true.
	*/
	virtual void BlitTextureBackground(const std::shared_ptr<Texture> &tex, const void *data, size_t data_size, uint32_t w, uint32_t h, uint32_t x = 0, uint32_t y = 0, TextureBackgroundBlit flag = TextureBackgroundBlitDefault) = 0;

	/// Copy the texture content to a picture.
	virtual bool CaptureTexture(const Texture &tex, Picture &pic) = 0;

	/// Generate the texture mip-map chain.
	virtual void GenerateTextureMipmap(Texture &tex) = 0;

	bool HasTexture(const std::string &name) const { return textures.Has(name); }
	std::shared_ptr<Texture> GetTexture(const std::string &name) const { return textures.Get(name); }
	std::shared_ptr<Texture> LoadTexture(const std::string &name, bool use_cache = true);

	/// Mark texture as in use.
	virtual void MarkTextureAsInUse(Texture &tex) = 0;
	/// @}

	/**
		@name Shader
		@{
	*/
	virtual std::shared_ptr<GpuShader> NewShader(const std::string &name = "Shader") = 0;

	bool HasShader(const std::string &name) const { return shaders.Has(name); }
	std::shared_ptr<GpuShader> GetShader(const std::string &name) const { return shaders.Get(name); }
	virtual std::shared_ptr<GpuShader> LoadShader(const std::string &name, const std::string &source, bool use_cache = true);
	virtual std::shared_ptr<GpuShader> LoadShader(const std::string &name, bool use_cache = true);

	virtual void CreateShader(const std::shared_ptr<GpuShader> &shader, const std::shared_ptr<Shader> &core_shader);
	virtual void FreeShader(GpuShader &shader) = 0;

	virtual GpuShaderVariable GetShaderVariable(const std::string &name) = 0;
	virtual GpuShaderVariable GetShaderBuiltin(ShaderVariable::Semantic semantic) = 0;

	virtual void SetShaderInt(const GpuShaderVariable &var, const int *v, size_t count = 1) = 0;
	virtual void SetShaderInt2(const GpuShaderVariable &var, const int *vec2, size_t count = 1) = 0;
	virtual void SetShaderInt3(const GpuShaderVariable &var, const int *vec3, size_t count = 1) = 0;
	virtual void SetShaderInt4(const GpuShaderVariable &var, const int *vec4, size_t count = 1) = 0;
	virtual void SetShaderUnsigned(const GpuShaderVariable &var, const uint32_t *v, size_t count = 1) = 0;
	virtual void SetShaderUnsigned2(const GpuShaderVariable &var, const uint32_t *vec2, size_t count = 1) = 0;
	virtual void SetShaderUnsigned3(const GpuShaderVariable &var, const uint32_t *vec3, size_t count = 1) = 0;
	virtual void SetShaderUnsigned4(const GpuShaderVariable &var, const uint32_t *vec4, size_t count = 1) = 0;
	virtual void SetShaderFloat(const GpuShaderVariable &var, const float *v, size_t count = 1) = 0;
	virtual void SetShaderFloat2(const GpuShaderVariable &var, const float *vec2, size_t count = 1) = 0;
	virtual void SetShaderFloat3(const GpuShaderVariable &var, const float *vec3, size_t count = 1) = 0;
	virtual void SetShaderFloat4(const GpuShaderVariable &var, const float *vec4, size_t count = 1) = 0;
	virtual void SetShaderMatrix3(const GpuShaderVariable &var, const Matrix3 *m, size_t count = 1) = 0;
	virtual void SetShaderMatrix4(const GpuShaderVariable &var, const Matrix4 *m, size_t count = 1) = 0;
	virtual void SetShaderMatrix44(const GpuShaderVariable &var, const Matrix44 *m, size_t count = 1) = 0;
	virtual void SetShaderTexture(const GpuShaderVariable &var, const Texture &t) = 0;

	virtual void SetShaderSystemBuiltins(float clock, const iVector2 &internal_resolution, uint32_t fx_scale, const Color &ambient_color, bool is_forward, bool fog_enabled, const Color &fog_color, float fog_near, float fog_far, Texture &depth_map) = 0;
	virtual void SetShaderCameraBuiltins(const Matrix4 &view_world, float z_near, float z_far, float zoom_factor, float eye) = 0;
	virtual void SetShaderTransformationBuiltins(const Matrix44 &view_pm, const Matrix4 &view_m, const Matrix4 &view_im, const Matrix4 *node_m, const Matrix4 *node_im, const Matrix4 &prv_view_m, const Matrix4 &prv_view_im, const Matrix4 *i_m, size_t count) = 0;
	virtual void SetShaderLightBuiltins(const Matrix4 &light_world, const Color &light_diffuse, const Color &light_specular, float range, float clip_dist, float cone_angle, float edge_angle, Texture *projection_map, const Matrix4 &view_world, Texture *shadow_map, float shadow_bias, float inv_shadow_map_size, const Color &shadow_color, size_t shadow_data_count, const Matrix4 *shadow_data_inv_world, const Matrix44 *shadow_data_projection_to_map, const float *shadow_data_slice_distance) = 0;
	virtual void SetShaderSkeletonValues(size_t skin_bone_count, const Matrix4 *skin_bone_matrices, const Matrix4 *skin_bone_previous_matrices, const uint16_t *skin_bone_idx) = 0;
	virtual void SetShaderPickingBuiltins(uint32_t uid) = 0;
	virtual void SetShaderValues(const std::vector<GpuShaderValue> &shader_values, const std::vector<GpuShaderValue> *additional_values = nullptr) = 0;
	/// @}

	/**
		@name Caps
		@{
	*/
protected:
	struct AdapterCaps {
		AdapterCaps() : can_resolve_msaa(true), tex_origin_is_top_left(true) {}

		GpuNPOTSupport npot_support{GpuNPOTFull};

		bool can_resolve_msaa : 1;
		bool tex_origin_is_top_left : 1;

		uint8_t max_anisotropy{4};
	};

	AdapterCaps adapter_caps;

	void FillAdapterCaps();

public:
	/// Query a caps on the renderer.
	virtual TypeValue QueryCaps(GpuCaps caps) { return TypeValue(); }
	/// Get static adapter caps.
	const AdapterCaps &GetAdapterCaps() const { return adapter_caps; }
	/// @}

	/**
		@name Configuration
		@{
	*/
	struct Config {
		uint8_t anisotropic_sample_count_default{4};
	};

	Config config;
	/// @}

	/**
		@name State functions
		@{
	*/
	enum FillMode {
		FillSolid,
		FillWireframe,
		FillLast
	};

	/// Set primitive fill mode.
	virtual void SetFillMode(FillMode mode) = 0;

	enum CullFunc {
		CullFront,
		CullBack,
		CullLast
	};

	/// Set culling function.
	virtual void SetCullFunc(CullFunc func) = 0;
	/// Enable culling.
	virtual void EnableCulling(bool enable) = 0;

	enum DepthFunc {
		DepthNever,
		DepthLess,
		DepthEqual,
		DepthLessEqual,
		DepthGreater,
		DepthNotEqual,
		DepthGreaterEqual,
		DepthAlways,
		DepthFuncLast
	};

	/// Set depth function.
	virtual void SetDepthFunc(DepthFunc func) = 0;
	/// Set depth read.
	virtual void EnableDepthTest(bool enable) = 0;
	/// Set depth write.
	virtual void EnableDepthWrite(bool enable) = 0;

	enum BlendFunc {
		BlendZero,
		BlendOne,
		BlendSrcAlpha,
		BlendOneMinusSrcAlpha,
		BlendDstAlpha,
		BlendOneMinusDstAlpha,
		BlendLast
	};

	/// Set blend function.
	virtual void SetBlendFunc(BlendFunc src, BlendFunc dst) = 0;
	/// Enable blend.
	virtual void EnableBlending(bool enable) = 0;

	/// Enable alpha to coverage.
	virtual void EnableAlphaToCoverage(bool enable) = 0;

	/// Set color write mask.
	virtual void SetColorMask(bool red, bool green, bool blue, bool alpha) = 0;

	/// Set default render states.
	void SetDefaultStates();

	/// Set index buffer object.
	virtual void SetIndexSource() = 0;
	virtual void SetIndexSource(GpuBuffer &buffer) = 0;
	/// Set vertex buffer object.
	virtual void SetVertexSource(GpuBuffer &buffer, const VertexLayout &layout) = 0;

	/// Get current shader object.
	virtual const std::shared_ptr<GpuShader> &GetShader() const = 0;
	/// Set current shader object.
	virtual bool SetShader(const std::shared_ptr<GpuShader> &shader) = 0;

	/// Set polygon fill depth offset.
	virtual void SetPolygonDepthOffset(float slope_scale, float bias) = 0;
	/// @}

	/**
		@name Resources
		@{
	*/
public:
	std::unique_ptr<GpuShaderCompiler> shader_compiler;

	/// Translate a shader from ISL to the target platform language.
	virtual bool TranslateShader(const Shader &shader, std::string &vertex_shader_src, std::string &pixel_shader_src) const = 0;

	struct Statistics {
		size_t available_video_memory{0};
	};

	Statistics stats;

	/**
		@name Helper functions
		@{
	*/
private:
	std::shared_ptr<GpuBuffer> quad_idx_buffer, quad_vtx_buffer;

protected:
	std::shared_ptr<GpuShader> single_texture_2d_shader;

public:
	/// Draw a shaded quad.
	void Quad(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &d, const Vector2 &uv_a, const Vector2 &uv_b, const Vector2 &uv_c, const Vector2 &uv_d);
	/// @}

	/**
		@name Window system integration
		@{
	*/
protected:
	Surface output_surface;

public:
	/// Create a new renderer surface from a window.
	virtual Surface NewOutputSurface(const Window &window) = 0;
	/// Set the output surface.
	virtual void SetOutputSurface(const Surface &surface) = 0;
	/// Free an output surface.
	virtual void DestroyOutputSurface(Surface &surface) = 0;
	/// Create an off screen output surface.
	virtual Surface NewOffscreenOutputSurface(int width, int height) = 0;
	/// Return the window native handle.
	virtual void *GetWindowHandle(Surface &surface) = 0;

	/// Return the current output surface.
	const Surface &GetOutputSurface() const { return output_surface; }
	/// Return the size of the output surface.
	virtual iVector2 GetOutputSurfaceSize() = 0;

	/// Return true if the current renderer surface has focus.
	virtual bool IsSurfaceInFocus() = 0;
	/// @}

protected:
	bool open = false;

	/// Open low-level.
	virtual bool OpenLowLevel(bool debug) = 0;
	/// Close low-level.
	virtual void CloseLowLevel() = 0;

public:
	/// Open video.
	bool Open(bool debug = false);
	/// Close video.
	void Close();

	/// Returns true if the renderer is open.
	bool IsOpen() const { return open; }

protected:
	RendererMatrices matrices, previous_matrices;

public:
	inline Matrix4 GetInverseViewMatrix() const { return matrices.iview; }
	inline Matrix4 GetInverseWorldMatrix() const { return matrices.iworld; }
	inline Matrix4 GetInversePreviousViewMatrix() const { return previous_matrices.iview; }

	/// Set the view matrix.
	void SetViewMatrix(const Matrix4 &view_matrix);
	/// Get the current view matrix.
	inline Matrix4 GetViewMatrix() const { return matrices.view; }
	/// Set the projection matrix.
	void SetProjectionMatrix(const Matrix44 &projection_matrix);
	/// Get the current projection matrix.
	inline Matrix44 GetProjectionMatrix() const { return matrices.projection; }
	/// Set the world matrix.
	void SetWorldMatrix(const Matrix4 &world_matrix);
	/// Get the current world matrix.
	inline Matrix4 GetWorldMatrix() const { return matrices.world; }
	/// Set the previous world matrix.
	void SetPreviousWorldMatrix(const Matrix4 &previous_world_matrix);
	/// Get the previous world matrix.
	inline Matrix4 GetPreviousWorldMatrix() const { return previous_matrices.world; }
	/// Set the previous view matrix.
	void SetPreviousViewMatrix(const Matrix4 &previous_view_matrix);
	/// Get the previous view matrix.
	inline Matrix4 GetPreviousViewMatrix() const { return previous_matrices.view; }

	/// Set identity world, view and projection matrices.
	void SetIdentityMatrices();
	/// Set world, view and projection matrices to render 2d.
	void Set2DMatrices(const Vector2 &resolution, bool y_up = true);
	void Set2DMatrices(bool y_origin_bottom = true);

	/**
		@short Return a screen-space coordinate as a 3d vertex.
		The returned vertex can be used to render screen-space primitives when
		all renderer matrices (view, world and projection) are set to identity.
		@see SetIdentityMatrices
	*/
	Vector3 ScreenVertex(int x, int y) const;

	/// Enable a user clipping plane.
	virtual void EnableClippingPlane(int idx) = 0;
	/// Disable a user clipping plane.
	virtual void DisableClippingPlane(int idx) = 0;
	/// Set a user clipping plane.
	virtual void SetClippingPlane(int idx, float a, float b, float c, float d) = 0;

protected:
	fRect clipping{-1, -1, -1, -1}, viewport{-1, -1, -1, -1};
	size_t frame_shown_counter{0};

public:
	/// Clear clipping rect.
	virtual void ClearClippingRect() = 0;
	/// Set clipping rect.
	virtual void SetClippingRect(const fRect &clipping_rect) = 0;
	/// Get clipping rect.
	virtual fRect GetClippingRect() const = 0;

	/// Set viewport.
	virtual void SetViewport(const fRect &) = 0;
	/// Get viewport.
	virtual fRect GetViewport() const = 0;

	/// Get viewport aspect ratio.
	inline Vector2 GetAspectRatio() const { return Vector2(viewport.GetWidth() / viewport.GetHeight(), 1); }

	/// Clear function
	enum ClearFunction {
		ClearColor = 0x01,
		ClearDepth = 0x02,
		ClearAll = 0xffff
	};

	/// Clear the render target.
	virtual void Clear(Color, float z = 1.f, ClearFunction = ClearAll) = 0;

	/// Draw elements.
	virtual void DrawElements(GpuPrimitiveType prim_type, size_t idx_count, IndexType idx_type = IndexUShort, size_t idx_offset = 0) = 0;
	/// Draw elements instanced.
	virtual void DrawElementsInstanced(size_t instance_count, GpuBuffer &instance_data, GpuPrimitiveType prim_type, size_t idx_count, IndexType idx_type = IndexUShort) = 0;

	/// Execute the command buffer.
	virtual void DrawFrame() = 0;
	/// Show the current frame.
	virtual void ShowFrame() = 0;

	/// Enable or disable vertical synchronization with the display when showing a frame.
	virtual void SetVSync(bool enabled) = 0;

	/// Capture the frame buffer (at point of call in GPU command list).
	virtual bool CaptureFramebuffer(Picture &out) = 0;

	/// Invalidate state cache. This function is useful when the graphic API render states have been changed outside of the renderer.
	virtual void InvalidateStateCache() = 0;

	/// Return the number of frame showed since the renderer creation.
	size_t GetFrameShownCount() const { return frame_shown_counter; }

	/// Push a new debug group.
	virtual void PushDebugGroup(const std::string &message) = 0;
	/// Pop the current debug group.
	virtual void PopDebugGroup() = 0;
};

extern Renderer *g_renderer;

void DrawBuffers(Renderer &renderer, size_t index_count, GpuBuffer &idx, GpuBuffer &vtx, VertexLayout &layout, IndexType idx_type = IndexUShort, GpuPrimitiveType prim_type = GpuPrimitiveTriangle);

} // namespace hg
