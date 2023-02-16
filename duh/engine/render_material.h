// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/gpu_shader.h"
#include "engine/material.h"
#include "engine/surface_shader.h"

namespace hg {

/// Render material
class RenderMaterial : public GpuResource {
public:
	explicit RenderMaterial(const std::string &name = std::string()) : GpuResource(name) {}

	/// Re-create from core material.
	void Create(RenderSystem &render_system, const std::shared_ptr<Material> &mat);
	/// Free material.
	void Free();

	/// Clone material.
	std::shared_ptr<RenderMaterial> Clone() const;

	bool IsReadyOrFailed() const override;

	inline const std::shared_ptr<SurfaceShader> &GetSurfaceShader() const { return surface_shader; }
	void SetSurfaceShader(const std::shared_ptr<SurfaceShader> &srf, const std::shared_ptr<Material> &mat = nullptr);

	std::vector<GpuShaderValue> values;

	/// Return a material value slot.
	GpuShaderValue *GetValue(const std::string &name);

private:
	void LinkSurfaceShader(const std::shared_ptr<Material> &core_mat);

	std::shared_ptr<SurfaceShader> surface_shader;
};

void RenderMaterialToMaterial(const RenderMaterial &render_mat, Material &mat);
bool SaveRenderMaterial(const RenderMaterial &render_mat, const std::string &path);

} // namespace hg
