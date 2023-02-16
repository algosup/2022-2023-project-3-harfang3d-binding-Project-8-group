// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/engine.h"
#include "engine/light_system.h"
#include "engine/render_draw.h"
#include "engine/render_system.h"
#include "engine/renderer.h"
#include "engine/set_shader_engine_values.h"
#include "foundation/call_task.h"
#include "foundation/matrix3.h"
#include "foundation/matrix4.h"
#include "foundation/matrix44.h"
#include "foundation/vector2.h"
#include "foundation/vector3.h"
#include "foundation/vector4.h"

namespace hg {

#define RSA_task_affinity nullptr, renderer_task_affinity

class RenderSystemAsync {
public:
	RenderSystemAsync(std::shared_ptr<RenderSystem> s) : render_system(std::move(s)) {}

	const std::shared_ptr<RenderSystem> &GetRenderSystem() const { return render_system; }

	std::future<RenderTechnique> GetRenderTechnique() { return run_call_with_return_value<RenderTechnique>(std::bind(&RenderSystem::GetRenderTechnique, shared_ref(render_system)), RSA_task_affinity); }
	void SetRenderTechnique(RenderTechnique technique, bool render_to_float_buffers = false) { run_call<void>(std::bind(&RenderSystem::SetRenderTechnique, shared_ref(render_system), technique, render_to_float_buffers), RSA_task_affinity); }

	std::future<iVector2> GetInternalResolution() const { return run_call_with_return_value<iVector2>(std::bind(&RenderSystem::GetInternalResolution, shared_ref(render_system)), RSA_task_affinity); }
	void SetInternalResolution(const iVector2 &resolution) { run_call<void>(std::bind(&RenderSystem::SetInternalResolution, shared_ref(render_system), resolution), RSA_task_affinity); }

	std::future<Vector2> GetViewportToInternalResolutionRatio() const { return run_call_with_return_value<Vector2>(std::bind(&RenderSystem::GetViewportToInternalResolutionRatio, shared_ref(render_system)), RSA_task_affinity); }

	void SetAA(uint32_t sample_count) { render_system->SetAA(sample_count); }
	void CreateShadowMaps(uint16_t resolution = 1024) { return run_call<void>(std::bind(&RenderSystem::CreateShadowMaps, shared_ref(render_system), resolution), RSA_task_affinity); }

private:
	void _SetView0(const Matrix4 &view, const Matrix44 &projection, Eye eye) { render_system->SetView(view, projection, eye); }
	void _SetView1(const Matrix4 &view, const Matrix4 &prev_view, const Matrix44 &projection, Eye eye) { render_system->SetView(view, prev_view, projection, eye); }

public:
	void SetView(const Matrix4 &view, const Matrix44 &projection, Eye eye = EyeMono) { run_call<void>(std::bind(&RenderSystemAsync::_SetView0, this, view, projection, eye), RSA_task_affinity); }
	void SetView(const Matrix4 &view, const Matrix4 &prev_view, const Matrix44 &projection, Eye eye = EyeMono) { run_call<void>(std::bind(&RenderSystemAsync::_SetView1, this, view, prev_view, projection, eye), RSA_task_affinity); }

	std::future<size_t> PurgeCache() { return run_call_with_return_value<size_t>(std::bind(&RenderSystem::PurgeCache, shared_ref(render_system)), RSA_task_affinity); }
	std::future<void> ClearCache() { return run_call_with_return_value<void>(std::bind(&RenderSystem::ClearCache, shared_ref(render_system)), RSA_task_affinity); }
	void RefreshCacheEntry(const std::string &name) { run_call<void>(std::bind(&RenderSystem::RefreshCacheEntry, shared_ref(render_system), name), RSA_task_affinity); }

	void DrawRasterFontBatch() { run_call<void>(std::bind(&RenderSystem::DrawRasterFontBatch, shared_ref(render_system)), RSA_task_affinity); }

	bool HasMaterial(const std::string &name) { return render_system->HasMaterial(name); }
	std::shared_ptr<RenderMaterial> GetMaterial(const std::string &name) { return render_system->GetMaterial(name); }
	std::shared_ptr<RenderMaterial> LoadMaterial(const std::string &name, bool use_cache = true) { return render_system->LoadMaterial(name, use_cache); }
	std::shared_ptr<RenderMaterial> LoadMaterial(const std::string &name, const std::string &source, DocumentFormat format = DocumentFormatUnknown, bool use_cache = true) { return render_system->LoadMaterial(name, source, format, use_cache); }
	std::shared_ptr<RenderMaterial> CreateMaterial(const std::shared_ptr<Material> &mat, bool use_cache = true) { return render_system->CreateMaterial(mat, use_cache); }

	bool HasGeometry(const std::string &name) { return render_system->HasGeometry(name); }
	std::shared_ptr<RenderGeometry> GetGeometry(const std::string &name) { return render_system->GetGeometry(name); }
	std::shared_ptr<RenderGeometry> LoadGeometry(const std::string &name, bool use_cache = true) { return render_system->LoadGeometry(name, use_cache); }
	std::shared_ptr<RenderGeometry> LoadGeometry(const std::string &name, const std::string &source, DocumentFormat format = DocumentFormatUnknown, bool use_cache = true) { return render_system->LoadGeometry(name, source, format, use_cache); }
	std::shared_ptr<RenderGeometry> CreateGeometry(const std::shared_ptr<Geometry> &geo, bool use_cache = true) { return render_system->CreateGeometry(geo, use_cache); }

	bool HasSurfaceShader(const std::string &name) { return render_system->HasSurfaceShader(name); }
	std::shared_ptr<SurfaceShader> GetSurfaceShader(const std::string &name) { return render_system->GetSurfaceShader(name); }
	std::shared_ptr<SurfaceShader> LoadSurfaceShader(const std::string &name, bool use_cache = true) { return render_system->LoadSurfaceShader(name, use_cache); }

	//
	void DrawLine(uint32_t count, const std::vector<Vector3> &vtx, const std::vector<Color> *color = nullptr, const std::vector<Vector2> *uv = nullptr) {
		run_call<void>(std::bind(&RenderSystem::DrawLine, shared_ref(render_system), count, memory_block<Vector3>(vtx.data(), memory_usage(vtx)),
						   memory_block<Color>(color ? color->data() : nullptr, color ? memory_usage(*color) : 0),
						   memory_block<Vector2>(uv ? uv->data() : nullptr, uv ? memory_usage(*uv) : 0), nullptr, 0),
			RSA_task_affinity);
	}

	void DrawTriangle(uint32_t count, const std::vector<Vector3> &vtx, const std::vector<Color> *color = nullptr, const std::vector<Vector2> *uv = nullptr) {
		run_call<void>(std::bind(&RenderSystem::DrawTriangle, shared_ref(render_system), count, memory_block<Vector3>(vtx.data(), memory_usage(vtx)),
						   memory_block<Color>(color ? color->data() : nullptr, color ? memory_usage(*color) : 0),
						   memory_block<Vector2>(uv ? uv->data() : nullptr, uv ? memory_usage(*uv) : 0), nullptr, 0),
			RSA_task_affinity);
	}

	void DrawSprite(uint32_t count, const std::vector<Vector3> &vtx, const std::vector<Color> *color = nullptr, const std::vector<float> *size = nullptr, float global_size = 1.f) {
		run_call<void>(std::bind(&RenderSystem::DrawSprite, shared_ref(render_system), count, memory_block<Vector3>(vtx.data(), memory_usage(vtx)),
						   memory_block<Color>(color ? color->data() : nullptr, color ? memory_usage(*color) : 0),
						   memory_block<float>(size ? size->data() : nullptr, size ? memory_usage(*size) : 0), global_size),
			RSA_task_affinity);
	}

	void DrawLineAuto(uint32_t count, const std::vector<Vector3> &vtx, const std::vector<Color> *color = nullptr, const std::vector<Vector2> *uv = nullptr, std::shared_ptr<Texture> texture = nullptr) {
		run_call<void>(std::bind(&RenderSystem::DrawLineAuto, shared_ref(render_system), count, memory_block<Vector3>(vtx.data(), memory_usage(vtx)),
						   memory_block<Color>(color ? color->data() : nullptr, color ? memory_usage(*color) : 0),
						   memory_block<Vector2>(uv ? uv->data() : nullptr, uv ? memory_usage(*uv) : 0), shared_ref(texture), nullptr, 0),
			RSA_task_affinity);
	}

	void DrawTriangleAuto(uint32_t count, const std::vector<Vector3> &vtx, const std::vector<Color> *color = nullptr, const std::vector<Vector2> *uv = nullptr, std::shared_ptr<Texture> texture = nullptr) {
		run_call<void>(std::bind(&RenderSystem::DrawTriangleAuto, shared_ref(render_system), count, memory_block<Vector3>(vtx.data(), memory_usage(vtx)),
						   memory_block<Color>(color ? color->data() : nullptr, color ? memory_usage(*color) : 0),
						   memory_block<Vector2>(uv ? uv->data() : nullptr, uv ? memory_usage(*uv) : 0), shared_ref(texture), nullptr, 0),
			RSA_task_affinity);
	}

	void DrawSpriteAuto(uint32_t count, const std::vector<Vector3> &vtx, const std::vector<Color> *color = nullptr, const std::vector<float> *size = nullptr, std::shared_ptr<Texture> texture = nullptr, float global_size = 1.f) {
		run_call<void>(std::bind(&RenderSystem::DrawSpriteAuto, shared_ref(render_system), count, memory_block<Vector3>(vtx.data(), memory_usage(vtx)),
						   shared_ref(texture), memory_block<Color>(color ? color->data() : nullptr, color ? memory_usage(*color) : 0),
						   memory_block<float>(size ? size->data() : nullptr, size ? memory_usage(*size) : 0), global_size),
			RSA_task_affinity);
	}

	void DrawQuad2D(const fRect &src, const fRect &dst) { run_call<void>(std::bind(&RenderSystem::DrawQuad2D, shared_ref(render_system), src, dst), RSA_task_affinity); }
	void DrawFullscreenQuad(const Vector2 &uv) { run_call<void>(std::bind(&RenderSystem::DrawFullscreenQuad, shared_ref(render_system), uv), RSA_task_affinity); }

	//
	void BeginDrawFrame() { run_call<bool>(std::bind(&RenderSystem::BeginDrawFrame, shared_ref(render_system)), RSA_task_affinity); }
	void DrawRenderables(const std::vector<std::shared_ptr<Renderable>> &renderables, const std::shared_ptr<LightSystem> &light_system) { run_call<void>(std::bind(&RenderSystem::DrawRenderables, shared_ref(render_system), renderables, shared_ref(light_system)), RSA_task_affinity); }
	void EndDrawFrame() { run_call<void>(std::bind(&RenderSystem::EndDrawFrame, shared_ref(render_system)), RSA_task_affinity); }

	std::future<bool> DrawRenderablesPicking(const std::vector<std::shared_ptr<Renderable>> &renderables, const std::shared_ptr<RenderTarget> &rt) { return run_call_with_return_value<bool>(std::bind(&hg::DrawRenderablesPicking, shared_ref(render_system), renderables, rt), RSA_task_affinity); }

	std::future<bool> Initialize(std::shared_ptr<Renderer> r, bool support_3d = true) { return run_call_with_return_value<bool>(std::bind(&RenderSystem::Initialize, shared_ref(render_system), r, support_3d), RSA_task_affinity); }
	std::future<void> Free() { return run_call_with_return_value<void>(std::bind(&RenderSystem::Free, shared_ref(render_system)), RSA_task_affinity); }

	void SetShaderEngineValues() { run_call<void>(std::bind(&hg::SetShaderEngineValues, shared_ref(render_system)), RSA_task_affinity); }

	//
private:
	void _SetSurfaceShaderInt(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<int> &v) { render_system->SetSurfaceShaderInt(*shader, name, v.data(), v.size()); }
	void _SetSurfaceShaderInt2(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<iVector2> &v) { render_system->SetSurfaceShaderInt2(*shader, name, &v[0].x, v.size()); }

	void _SetSurfaceShaderFloat(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<float> &v) { render_system->SetSurfaceShaderFloat(*shader, name, v.data(), v.size()); }
	void _SetSurfaceShaderFloat2(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<Vector2> &v) { render_system->SetSurfaceShaderFloat2(*shader, name, &v[0].x, v.size()); }
	void _SetSurfaceShaderFloat3(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<Vector3> &v) { render_system->SetSurfaceShaderFloat3(*shader, name, &v[0].x, v.size()); }
	void _SetSurfaceShaderFloat4(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<Vector4> &v) { render_system->SetSurfaceShaderFloat4(*shader, name, &v[0].x, v.size()); }

	void _SetSurfaceShaderMatrix3(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<Matrix3> &v) { render_system->SetSurfaceShaderMatrix3(*shader, name, v.data(), v.size()); }
	void _SetSurfaceShaderMatrix4(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<Matrix4> &v) { render_system->SetSurfaceShaderMatrix4(*shader, name, v.data(), v.size()); }
	void _SetSurfaceShaderMatrix44(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<Matrix44> &v) { render_system->SetSurfaceShaderMatrix44(*shader, name, v.data(), v.size()); }

	void _SetSurfaceShaderTexture(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::shared_ptr<Texture> &t) { render_system->SetSurfaceShaderTexture(*shader, name, t); }

public:
	void SetSurfaceShaderInt(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<int> &v) { run_call<void>(std::bind(&RenderSystemAsync::_SetSurfaceShaderInt, this, shader, name, v), RSA_task_affinity); }
	void SetSurfaceShaderInt2(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<iVector2> &v) { run_call<void>(std::bind(&RenderSystemAsync::_SetSurfaceShaderInt2, this, shader, name, v), RSA_task_affinity); }

	void SetSurfaceShaderFloat(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<float> &v) { run_call<void>(std::bind(&RenderSystemAsync::_SetSurfaceShaderFloat, this, shader, name, v), RSA_task_affinity); }
	void SetSurfaceShaderFloat2(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<Vector2> &v) { run_call<void>(std::bind(&RenderSystemAsync::_SetSurfaceShaderFloat2, this, shader, name, v), RSA_task_affinity); }
	void SetSurfaceShaderFloat3(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<Vector3> &v) { run_call<void>(std::bind(&RenderSystemAsync::_SetSurfaceShaderFloat3, this, shader, name, v), RSA_task_affinity); }
	void SetSurfaceShaderFloat4(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<Vector4> &v) { run_call<void>(std::bind(&RenderSystemAsync::_SetSurfaceShaderFloat4, this, shader, name, v), RSA_task_affinity); }

	void SetSurfaceShaderMatrix3(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<Matrix3> &v) { run_call<void>(std::bind(&RenderSystemAsync::_SetSurfaceShaderMatrix3, this, shader, name, v), RSA_task_affinity); }
	void SetSurfaceShaderMatrix4(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<Matrix4> &v) { run_call<void>(std::bind(&RenderSystemAsync::_SetSurfaceShaderMatrix4, this, shader, name, v), RSA_task_affinity); }
	void SetSurfaceShaderMatrix44(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::vector<Matrix44> &v) { run_call<void>(std::bind(&RenderSystemAsync::_SetSurfaceShaderMatrix44, this, shader, name, v), RSA_task_affinity); }

	void SetSurfaceShaderTexture(const std::shared_ptr<SurfaceShader> &shader, const std::string &name, const std::shared_ptr<Texture> &t) { run_call<void>(std::bind(&RenderSystemAsync::_SetSurfaceShaderTexture, this, shader, name, t), RSA_task_affinity); }

private:
	std::shared_ptr<RenderSystem> render_system;
};

} // namespace hg
