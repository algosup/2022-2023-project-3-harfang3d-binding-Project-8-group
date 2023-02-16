// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/engine.h"
#include "engine/picture.h"
#include "engine/renderer.h"
#include "engine/vertex_layout.h"
#include "foundation/binary_data.h"
#include "foundation/call_task.h"
#include "foundation/cext.h"
#include "foundation/color.h"
#include "platform/window_system.h"

namespace hg {

#define RA_task_affinity nullptr, renderer_task_affinity

/// GPU based renderer async interface
class RendererAsync {
public:
	RendererAsync(std::shared_ptr<Renderer> renderer_) : renderer(renderer_) {}

	const std::shared_ptr<Renderer> &GetRenderer() const { return renderer; }

	std::future<size_t> PurgeCache() { return run_call_with_return_value<size_t>(std::bind(&Renderer::PurgeCache, renderer.get()), RA_task_affinity); }
	std::future<void> ClearCache() { return run_call_with_return_value<void>(std::bind(&Renderer::ClearCache, renderer.get()), RA_task_affinity); }
	void RefreshCacheEntry(const std::string &name) { run_call<void>(std::bind(&Renderer::RefreshCacheEntry, renderer.get(), name), RA_task_affinity); }

private:
	std::shared_ptr<Renderer> renderer;

	static void SetRenderTargetColorTexture_adapter(Renderer &r, const std::shared_ptr<RenderTarget> &rt, std::shared_ptr<Texture> tex) { r.SetRenderTargetColorTexture(*rt, &tex, 1); }

	static void ReadRenderTargetColorPixels_adapter(Renderer &r, const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Picture> &out, const iRect &rect) {
		out->AllocAs(rect.GetWidth(), rect.GetHeight(), PictureRGBA8);
		r.ReadRenderTargetColorPixels(rt, out->GetData(), rect);
	}

	/**
		@name Render target
		@{
	*/
public:
	std::shared_ptr<RenderTarget> NewRenderTarget() { return renderer->NewRenderTarget(); }

	void SetRenderTargetColorTexture(const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &tex) {
		run_call<void>(std::bind(&RendererAsync::SetRenderTargetColorTexture_adapter, shared_ref(renderer), rt, tex), RA_task_affinity);
	}

	void SetRenderTargetDepthTexture(const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &tex) {
		run_call<void>(std::bind(&Renderer::SetRenderTargetDepthTexture, renderer, shared_ref(rt), tex), RA_task_affinity);
	}

	void BlitRenderTarget(const std::shared_ptr<RenderTarget> &src_rt, const std::shared_ptr<RenderTarget> &dst_rt, const iRect &src, const iRect &dst, bool color = true, bool depth = true) {
		run_call<void>(std::bind(&Renderer::BlitRenderTarget, shared_ref(renderer), src_rt, dst_rt, src, dst, color, depth), RA_task_affinity);
	}

	void ReadRenderTargetColorPixels(const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Picture> &out, const iRect &rect) {
		run_call<void>(std::bind(&RendererAsync::ReadRenderTargetColorPixels_adapter, shared_ref(renderer), rt, out, rect), RA_task_affinity);
	}

	void ClearRenderTarget() { run_call<void>(std::bind(&Renderer::ClearRenderTarget, shared_ref(renderer)), RA_task_affinity); }
	void SetRenderTarget(const std::shared_ptr<RenderTarget> &rt) { run_call<void>(std::bind(&Renderer::SetRenderTarget, shared_ref(renderer), rt), RA_task_affinity); }

	std::future<bool> CheckRenderTarget() {
		return run_call_with_return_value<bool>(std::bind(&Renderer::CheckRenderTarget, shared_ref(renderer)), RA_task_affinity);
	}

	std::future<bool> CreateRenderTarget(const std::shared_ptr<RenderTarget> &rt) {
		return run_call_with_return_value<bool>(std::bind(&Renderer::CreateRenderTarget, shared_ref(renderer), shared_ref(rt)), RA_task_affinity);
	}

	void FreeRenderTarget(const std::shared_ptr<RenderTarget> &rt) {
		run_call<void>(std::bind(&Renderer::FreeRenderTarget, shared_ref(renderer), shared_ref(rt)), RA_task_affinity);
	}

	std::future<std::shared_ptr<RenderTarget>> GetRenderTarget() {
		return run_call_with_return_value<std::shared_ptr<RenderTarget>>(std::bind(&Renderer::GetRenderTarget, shared_ref(renderer)), RA_task_affinity);
	}

	/**
		@name Buffer
		@{
	*/
private:
	static bool CreateBuffer_adapter(Renderer &r, const std::shared_ptr<GpuBuffer> &vbo, BinaryData &data, GpuBufferType type, GpuBufferUsage usage) {
		return r.CreateBuffer(*vbo, data.GetData(), data.GetDataSize(), type, usage);
	}

	static bool UpdateBuffer_adapter(Renderer &r, const std::shared_ptr<GpuBuffer> &vbo, BinaryData &data, size_t start = 0) {
		return r.UpdateBuffer(*vbo, data.GetData(), start, data.GetDataSize());
	}

public:
	std::shared_ptr<GpuBuffer> NewBuffer() { return renderer->NewBuffer(); }

	std::future<size_t> GetBufferSize(std::shared_ptr<GpuBuffer> buf) {
		return run_call_with_return_value<size_t>(std::bind(&Renderer::GetBufferSize, shared_ref(renderer), shared_ref(buf)), RA_task_affinity);
	}

	std::future<void *> MapBuffer(std::shared_ptr<GpuBuffer> buf) {
		return run_call_with_return_value<void *>(std::bind(&Renderer::MapBuffer, shared_ref(renderer), shared_ref(buf)), RA_task_affinity);
	}

	void UnmapBuffer(std::shared_ptr<GpuBuffer> buf) { run_call<void>(std::bind(&Renderer::UnmapBuffer, shared_ref(renderer), shared_ref(buf)), RA_task_affinity); }

	std::future<bool> UpdateBuffer(std::shared_ptr<GpuBuffer> buf, const void *p, size_t start = 0, size_t size = 0) {
		return run_call_with_return_value<bool>(std::bind(&RendererAsync::UpdateBuffer_adapter, shared_ref(renderer), buf, BinaryData(p, size), start),
			RA_task_affinity);
	}

	std::future<bool> UpdateBuffer(std::shared_ptr<GpuBuffer> buf, BinaryData &data, size_t start = 0) {
		return run_call_with_return_value<bool>(std::bind(&RendererAsync::UpdateBuffer_adapter, shared_ref(renderer), buf, data, start), RA_task_affinity);
	}

	std::future<bool> CreateBuffer(std::shared_ptr<GpuBuffer> buf, const void *data, size_t size, GpuBufferType type, GpuBufferUsage usage = GpuBufferStatic) {
		return run_call_with_return_value<bool>(std::bind(&RendererAsync::CreateBuffer_adapter, shared_ref(renderer), buf, BinaryData(data, size), type, usage),
			RA_task_affinity);
	}

	std::future<bool> CreateBuffer(std::shared_ptr<GpuBuffer> buf, const BinaryData &data, GpuBufferType type, GpuBufferUsage usage = GpuBufferStatic) {
		return run_call_with_return_value<bool>(std::bind(&RendererAsync::CreateBuffer_adapter, shared_ref(renderer), buf, data, type, usage), RA_task_affinity);
	}

	void FreeBuffer(std::shared_ptr<GpuBuffer> buf) { run_call<void>(bind(&Renderer::FreeBuffer, shared_ref(renderer), shared_ref(buf)), RA_task_affinity); }

	/**
		@name Texture
		@{
	*/
private:
	static void BlitTexture1(Renderer &r, Texture &t, const BinaryData &data, uint32_t w, uint32_t h, uint32_t x, uint32_t y) {
		r.BlitTexture(t, data.GetData(), data.GetDataSize(), w, h, x, y);
	}

	static bool CreateTexture1(Renderer &r, Texture &t, int w, int h, TextureFormat f, TextureAA aa, TextureUsage usage, bool mip) {
		return r.CreateTexture(t, nullptr, 0, w, h, f, aa, usage, mip);
	}

	static bool CreateTexture2(Renderer &r, Texture &t, Picture &p, TextureUsage usage, bool mip) { return r.CreateTexture(t, p, usage, mip); }

	static bool CreateTexture3(Renderer &r, Texture &t, BinaryData &data, int w, int h, TextureFormat f, TextureAA aa, TextureUsage usage, bool mip) {
		return r.CreateTexture(t, data.GetData(), data.GetDataSize(), w, h, f, aa, usage, mip);
	}

public:
	std::shared_ptr<Texture> NewTexture(const std::string &name = "Texture") { return renderer->NewTexture(name); }

	std::shared_ptr<Texture> NewShadowMap(int width, int height, const std::string &name = "Shadow") {
		return run_call_with_return_value<std::shared_ptr<Texture>>(
			std::bind(&Renderer::NewShadowMap, shared_ref(renderer), width, height, name), RA_task_affinity)
			.get();
	}

	std::future<bool> CreateTexture(std::shared_ptr<Texture> tex, int width, int height, TextureFormat format = TextureRGBA8, TextureAA aa = TextureNoAA,
		TextureUsage usage = TextureUsageDefault, bool mipmapped = true) {
		return run_call_with_return_value<bool>(
			std::bind(&RendererAsync::CreateTexture1, shared_ref(renderer), shared_ref(tex), width, height, format, aa, usage, mipmapped), RA_task_affinity);
	}

	std::future<bool> CreateTexture(std::shared_ptr<Texture> tex, BinaryData &data, int width, int height, TextureFormat format = TextureRGBA8, TextureAA aa = TextureNoAA,
		TextureUsage usage = TextureUsageDefault, bool mipmapped = true) {
		return run_call_with_return_value<bool>(
			std::bind(&RendererAsync::CreateTexture3, shared_ref(renderer), shared_ref(tex), data, width, height, format, aa, usage, mipmapped), RA_task_affinity);
	}

	void FreeTexture(std::shared_ptr<Texture> tex) { run_call<void>(std::bind(&Renderer::FreeTexture, shared_ref(renderer), shared_ref(tex)), RA_task_affinity); }

	void BlitTexture(std::shared_ptr<Texture> tex, const BinaryData &data, uint32_t width, uint32_t height, uint32_t x = 0, uint32_t y = 0) {
		run_call<void>(std::bind(&RendererAsync::BlitTexture1, shared_ref(renderer), shared_ref(tex), data, width, height, x, y), RA_task_affinity);
	}

	void ResizeTexture(std::shared_ptr<Texture> tex, uint32_t w, uint32_t h) {
		run_call<void>(std::bind(&Renderer::ResizeTexture, shared_ref(renderer), shared_ref(tex), w, h), RA_task_affinity);
	}

	void BlitTextureBackground(std::shared_ptr<Texture> tex, const BinaryData &data, uint32_t w, uint32_t h, uint32_t x = 0, uint32_t y = 0,
		TextureBackgroundBlit flags = TextureBackgroundBlitDefault) {
		renderer->BlitTextureBackground(tex, data.GetData(), data.GetDataSize(), w, h, x, y, flags);
	}

	std::future<bool> CaptureTexture(std::shared_ptr<Texture> tex, std::shared_ptr<Picture> out) {
		return run_call_with_return_value<bool>(std::bind(&Renderer::CaptureTexture, shared_ref(renderer), shared_ref(tex), shared_ref(out)), RA_task_affinity);
	}

	void GenerateTextureMipmap(std::shared_ptr<Texture> tex) {
		run_call<void>(bind(&Renderer::GenerateTextureMipmap, shared_ref(renderer), shared_ref(tex)), RA_task_affinity);
	}

	std::future<bool> CreateTexture(std::shared_ptr<Texture> tex, std::shared_ptr<Picture> p, TextureUsage usage = TextureUsageDefault, bool mipmapped = true) {
		return run_call_with_return_value<bool>(
			std::bind(&RendererAsync::CreateTexture2, shared_ref(renderer), shared_ref(tex), shared_ref(p), usage, mipmapped), RA_task_affinity);
	}

	std::shared_ptr<Texture> LoadTexture(const std::string &name, bool use_cache = true) { return renderer->LoadTexture(name, use_cache); }

	const char *GetNativeTextureExt() const { return renderer->GetNativeTextureExt(); }

	/**
		@name Shader
		@{
	*/
private:
	static void SetIndexSource0(Renderer &r) { r.SetIndexSource(); }
	static void SetIndexSource1(Renderer &r, GpuBuffer &b) { r.SetIndexSource(b); }

	static std::shared_ptr<GpuShader> GetShader_adapter(Renderer &r) { return r.GetShader(); }

	void _SetShaderTexture(const GpuShaderVariable &var, const std::shared_ptr<Texture> &tex) { renderer->SetShaderTexture(var, *tex); }

public:
	std::shared_ptr<GpuShader> LoadShader(const std::string &name, bool use_cache = true) { return renderer->LoadShader(name, use_cache); }
	std::shared_ptr<GpuShader> LoadShader(const std::string &name, const std::string& source, bool use_cache = true) { return renderer->LoadShader(name, source, use_cache); }
	void FreeShader(const std::shared_ptr<GpuShader> &s) { run_call<void>(std::bind(&Renderer::FreeShader, shared_ref(renderer), shared_ref(s)), RA_task_affinity); }

	std::future<GpuShaderVariable> GetShaderVariable(const std::string &name) { return run_call_with_return_value<GpuShaderVariable>(std::bind(&Renderer::GetShaderVariable, shared_ref(renderer), name), RA_task_affinity); }
	std::future<GpuShaderVariable> GetShaderBuiltin(ShaderVariable::Semantic semantic) { return run_call_with_return_value<GpuShaderVariable>(std::bind(&Renderer::GetShaderBuiltin, shared_ref(renderer), semantic), RA_task_affinity); }

	void SetShaderInt(const GpuShaderVariable &var, const int *v, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderInt, shared_ref(renderer), var, v, count), RA_task_affinity); }
	void SetShaderInt2(const GpuShaderVariable &var, const int *vec2, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderInt2, shared_ref(renderer), var, vec2, count), RA_task_affinity); }
	void SetShaderInt3(const GpuShaderVariable &var, const int *vec3, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderInt3, shared_ref(renderer), var, vec3, count), RA_task_affinity); }
	void SetShaderInt4(const GpuShaderVariable &var, const int *vec4, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderInt4, shared_ref(renderer), var, vec4, count), RA_task_affinity); }
	void SetShaderUnsigned(const GpuShaderVariable &var, const uint32_t *v, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderUnsigned, shared_ref(renderer), var, v, count), RA_task_affinity); }
	void SetShaderUnsigned2(const GpuShaderVariable &var, const uint32_t *vec2, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderUnsigned2, shared_ref(renderer), var, vec2, count), RA_task_affinity); }
	void SetShaderUnsigned3(const GpuShaderVariable &var, const uint32_t *vec3, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderUnsigned3, shared_ref(renderer), var, vec3, count), RA_task_affinity); }
	void SetShaderUnsigned4(const GpuShaderVariable &var, const uint32_t *vec4, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderUnsigned4, shared_ref(renderer), var, vec4, count), RA_task_affinity); }
	void SetShaderFloat(const GpuShaderVariable &var, const float *v, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderFloat, shared_ref(renderer), var, v, count), RA_task_affinity); }
	void SetShaderFloat2(const GpuShaderVariable &var, const float *vec2, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderFloat2, shared_ref(renderer), var, vec2, count), RA_task_affinity); }
	void SetShaderFloat3(const GpuShaderVariable &var, const float *vec3, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderFloat3, shared_ref(renderer), var, vec3, count), RA_task_affinity); }
	void SetShaderFloat4(const GpuShaderVariable &var, const float *vec4, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderFloat4, shared_ref(renderer), var, vec4, count), RA_task_affinity); }
	void SetShaderMatrix3(const GpuShaderVariable &var, const Matrix3 *m, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderMatrix3, shared_ref(renderer), var, m, count), RA_task_affinity); }
	void SetShaderMatrix4(const GpuShaderVariable &var, const Matrix4 *m, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderMatrix4, shared_ref(renderer), var, m, count), RA_task_affinity); }
	void SetShaderMatrix44(const GpuShaderVariable &var, const Matrix44 *m, size_t count = 1) { run_call<void>(std::bind(&Renderer::SetShaderMatrix44, shared_ref(renderer), var, m, count), RA_task_affinity); }
	void SetShaderTexture(const GpuShaderVariable &var, const std::shared_ptr<Texture> &tex) { run_call<void>(std::bind(&RendererAsync::_SetShaderTexture, this, var, tex), RA_task_affinity); }

	/**
		@name State functions
		@{
	*/
	void SetFillMode(Renderer::FillMode fill_mode) { run_call<void>(std::bind(&Renderer::SetFillMode, shared_ref(renderer), fill_mode), RA_task_affinity); }
	void SetCullFunc(Renderer::CullFunc cull_func) { run_call<void>(std::bind(&Renderer::SetCullFunc, shared_ref(renderer), cull_func), RA_task_affinity); }
	void EnableCulling(bool enable) { run_call<void>(std::bind(&Renderer::EnableCulling, shared_ref(renderer), enable), RA_task_affinity); }

	void SetDepthFunc(Renderer::DepthFunc depth_func) {
		run_call<void>(std::bind(&Renderer::SetDepthFunc, shared_ref(renderer), depth_func), RA_task_affinity);
	}

	void EnableDepthTest(bool enable) { run_call<void>(std::bind(&Renderer::EnableDepthTest, shared_ref(renderer), enable), RA_task_affinity); }
	void EnableDepthWrite(bool enable) { run_call<void>(std::bind(&Renderer::EnableDepthWrite, shared_ref(renderer), enable), RA_task_affinity); }

	void SetBlendFunc(Renderer::BlendFunc src, Renderer::BlendFunc dst) {
		run_call<void>(std::bind(&Renderer::SetBlendFunc, shared_ref(renderer), src, dst), RA_task_affinity);
	}

	void EnableBlending(bool enable) { run_call<void>(std::bind(&Renderer::EnableBlending, shared_ref(renderer), enable), RA_task_affinity); }
	void EnableAlphaToCoverage(bool enable) { run_call<void>(std::bind(&Renderer::EnableAlphaToCoverage, shared_ref(renderer), enable), RA_task_affinity); }
	void SetDefaultStates() { run_call<void>(std::bind(&Renderer::SetDefaultStates, shared_ref(renderer)), RA_task_affinity); }
	void SetIndexSource() { run_call<void>(std::bind(&RendererAsync::SetIndexSource0, shared_ref(renderer)), RA_task_affinity); }

	void SetIndexSource(const std::shared_ptr<GpuBuffer> &vbo) {
		run_call<void>(std::bind(&RendererAsync::SetIndexSource1, shared_ref(renderer), shared_ref(vbo)), RA_task_affinity);
	}

	void SetVertexSource(const std::shared_ptr<GpuBuffer> &vbo, const VertexLayout &layout) {
		run_call<void>(std::bind(&Renderer::SetVertexSource, shared_ref(renderer), shared_ref(vbo), layout), RA_task_affinity);
	}

	std::future<std::shared_ptr<GpuShader>> GetShader() { return run_call_with_return_value<std::shared_ptr<GpuShader>>(std::bind(&RendererAsync::GetShader_adapter, shared_ref(renderer)), RA_task_affinity); }

	std::future<bool> SetShader(std::shared_ptr<GpuShader> shader) {
		return run_call_with_return_value<bool>(std::bind(&Renderer::SetShader, shared_ref(renderer), shader), RA_task_affinity);
	}

	/**
		@name Window system
		@{
	*/
private:
	static void Set2DMatrices0(Renderer &r, bool y) { r.Set2DMatrices(y); }

	static void FitViewportToOutputSurface_(Renderer &r) {
		auto size = r.GetOutputSurfaceSize();
		r.SetViewport(fRect(0, 0, float(size.x), float(size.y)));
	}

public:
	std::future<Window> NewWindow(int w, int h, int bpp, Window::Visibility vis) {
		return run_call_with_return_value<Window>([w, h, bpp, vis] { return hg::NewWindow(w, h, bpp, vis); }, RA_task_affinity);
	}

	std::future<Surface> NewOutputSurface(const Window &win) {
		return run_call_with_return_value<Surface>(std::bind(&Renderer::NewOutputSurface, shared_ref(renderer), std::ref(win)), RA_task_affinity);
	}
	std::future<Surface> NewOffscreenOutputSurface(int width, int height) {
		return run_call_with_return_value<Surface>(std::bind(&Renderer::NewOffscreenOutputSurface, shared_ref(renderer), width, height), RA_task_affinity);
	}

	void SetOutputSurface(const Surface &srf) {
		run_call<void>(std::bind(&Renderer::SetOutputSurface, shared_ref(renderer), std::ref(srf)), RA_task_affinity);
	}

	void DestroyOutputSurface(Surface &srf) { run_call<void>(std::bind(&Renderer::DestroyOutputSurface, shared_ref(renderer), std::ref(srf)), RA_task_affinity); }

	std::future<Surface> GetOutputSurface() { return run_call_with_return_value<Surface>(std::bind(&Renderer::GetOutputSurface, shared_ref(renderer)), RA_task_affinity); }
	
	std::future<void*> GetWindowHandle(Surface &srf) { return run_call_with_return_value<void*>(std::bind(&Renderer::GetWindowHandle, shared_ref(renderer), std::ref(srf)), RA_task_affinity); }

	std::future<iVector2> GetOutputSurfaceSize() { return run_call_with_return_value<iVector2>(std::bind(&Renderer::GetOutputSurfaceSize, shared_ref(renderer)), RA_task_affinity); }

	void FitViewportToOutputSurface() { run_call<void>(std::bind(&RendererAsync::FitViewportToOutputSurface_, shared_ref(renderer)), RA_task_affinity); }

	void UpdateWindow(const Window &win) {
		return run_call<void>([win] { hg::UpdateWindow(win); }, RA_task_affinity);
	}
	/// @}

	std::future<bool> Open(bool debug = false) { return run_call_with_return_value<bool>(std::bind(&Renderer::Open, shared_ref(renderer), debug), RA_task_affinity); }

	std::future<void> Close() { return run_call_with_return_value<void>(std::bind(&Renderer::Close, shared_ref(renderer)), RA_task_affinity); }

	std::future<Matrix4> GetInverseViewMatrix() {
		return run_call_with_return_value<Matrix4>(std::bind(&Renderer::GetInverseViewMatrix, shared_ref(renderer)), RA_task_affinity);
	}

	std::future<Matrix4> GetInverseWorldMatrix() {
		return run_call_with_return_value<Matrix4>(std::bind(&Renderer::GetInverseWorldMatrix, shared_ref(renderer)), RA_task_affinity);
	}

	void SetViewMatrix(const Matrix4 &view) { run_call<void>(std::bind(&Renderer::SetViewMatrix, shared_ref(renderer), view), RA_task_affinity); }

	std::future<Matrix4> GetViewMatrix() {
		return run_call_with_return_value<Matrix4>(std::bind(&Renderer::GetViewMatrix, shared_ref(renderer)), RA_task_affinity);
	}

	void SetProjectionMatrix(const Matrix44 &projection) {
		run_call<void>(std::bind(&Renderer::SetProjectionMatrix, shared_ref(renderer), projection), RA_task_affinity);
	}

	std::future<Matrix44> GetProjectionMatrix() {
		return run_call_with_return_value<Matrix44>(std::bind(&Renderer::GetProjectionMatrix, shared_ref(renderer)), RA_task_affinity);
	}

	void SetWorldMatrix(const Matrix4 &world) { run_call<void>(std::bind(&Renderer::SetWorldMatrix, shared_ref(renderer), world), RA_task_affinity); }

	std::future<Matrix4> GetWorldMatrix() {
		return run_call_with_return_value<Matrix4>(std::bind(&Renderer::GetWorldMatrix, shared_ref(renderer)), RA_task_affinity);
	}

	void SetPreviousViewMatrix(const Matrix4 &view) { run_call<void>(std::bind(&Renderer::SetPreviousViewMatrix, shared_ref(renderer), view), RA_task_affinity); }

	std::future<Matrix4> GetPreviousViewMatrix() {
		return run_call_with_return_value<Matrix4>(std::bind(&Renderer::GetPreviousViewMatrix, shared_ref(renderer)), RA_task_affinity);
	}

	void SetIdentityMatrices() { run_call<void>(std::bind(&Renderer::SetIdentityMatrices, shared_ref(renderer)), RA_task_affinity); }
	void Set2DMatrices(bool reverse_y = true) { run_call<void>(std::bind(&RendererAsync::Set2DMatrices0, shared_ref(renderer), reverse_y), RA_task_affinity); }

	void EnableClippingPlane(int idx) { run_call<void>(bind(&Renderer::EnableClippingPlane, shared_ref(renderer), idx), RA_task_affinity); }
	void DisableClippingPlane(int idx) { run_call<void>(bind(&Renderer::DisableClippingPlane, shared_ref(renderer), idx), RA_task_affinity); }

	void SetClippingPlane(int idx, float a, float b, float c, float d) {
		run_call<void>(bind(&Renderer::SetClippingPlane, shared_ref(renderer), idx, a, b, c, d), RA_task_affinity);
	}

private:
	bool CaptureFramebuffer_adapter(Picture &p) { return renderer->CaptureFramebuffer(p); }

public:
	void ClearClippingRect() { run_call<void>(std::bind(&Renderer::ClearClippingRect, shared_ref(renderer)), RA_task_affinity); }
	void SetClippingRect(const fRect &clipping_rect) {
		run_call<void>(std::bind(&Renderer::SetClippingRect, shared_ref(renderer), clipping_rect), RA_task_affinity);
	}
	std::future<Rect<float>> GetClippingRect() {
		return run_call_with_return_value<Rect<float>>(std::bind(&Renderer::GetClippingRect, shared_ref(renderer)), RA_task_affinity);
	}

	void SetViewport(const fRect &viewport_rect) { run_call<void>(std::bind(&Renderer::SetViewport, shared_ref(renderer), viewport_rect), RA_task_affinity); }

	std::future<Rect<float>> GetViewport() {
		return run_call_with_return_value<Rect<float>>(std::bind(&Renderer::GetViewport, shared_ref(renderer)), RA_task_affinity);
	}

	std::future<Vector2> GetAspectRatio() const {
		return run_call_with_return_value<Vector2>(std::bind(&Renderer::GetAspectRatio, shared_ref(renderer)), RA_task_affinity);
	}

	void Clear(Color color, float z = 1.f, Renderer::ClearFunction clear_func = Renderer::ClearAll) {
		run_call<void>(std::bind(&Renderer::Clear, shared_ref(renderer), color, z, clear_func), RA_task_affinity);
	}

	void DrawBuffers(uint32_t index_count, const std::shared_ptr<GpuBuffer> &idx, const std::shared_ptr<GpuBuffer> &vtx, VertexLayout &layout, IndexType idx_type = IndexUShort,
		GpuPrimitiveType prim_type = GpuPrimitiveTriangle) {
		run_call<void>(std::bind(&hg::DrawBuffers, shared_ref(renderer), index_count, shared_ref(idx), shared_ref(vtx), layout, idx_type, prim_type),
			RA_task_affinity);
	}

	std::future<void> DrawFrame() { return run_call_with_return_value<void>(std::bind(&Renderer::DrawFrame, shared_ref(renderer)), RA_task_affinity); }
	std::future<void> ShowFrame() { return run_call_with_return_value<void>(std::bind(&Renderer::ShowFrame, shared_ref(renderer)), RA_task_affinity); }

	void SetVSync(bool enabled) { run_call<void>(std::bind(&Renderer::SetVSync, shared_ref(renderer), enabled), RA_task_affinity); }

	std::future<bool> CaptureFramebuffer(const std::shared_ptr<Picture> &out) {
		return run_call_with_return_value<bool>(std::bind(&Renderer::CaptureFramebuffer, shared_ref(renderer), shared_ref(out)), RA_task_affinity);
	}

	void PushDebugGroup(const std::string &msg) { return run_call<void>(std::bind(&Renderer::PushDebugGroup, shared_ref(renderer), msg), RA_task_affinity); }
	void PopDebugGroup() { return run_call<void>(std::bind(&Renderer::PopDebugGroup, shared_ref(renderer)), RA_task_affinity); }
};

} // namespace hg
