// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/gpu_resource.h"
#include "engine/render_pass.h"
#include "engine/renderable_context.h"
#include "foundation/vector4.h"

namespace hg {

class RenderSystem;

struct Shader;
struct GpuShader;
struct ShaderVariableTable;
struct Texture;

// Variants of a shader used by the rendering pipeline to draw primitives.
class SurfaceShader : public GpuResource {
public:
	explicit SurfaceShader(const std::string &name = std::string()) : GpuResource(name) {}

	uint32_t GetSurfaceDrawState() const { return surface_draw_state; }

	bool Create(RenderSystem &render_system, const std::shared_ptr<Shader> &shader);
	void Free();

	std::shared_ptr<GpuShader> pass_shaders[2][RenderPass::ShaderCount]; // non-instantiated/instantiated

	/**
		@short Return the list of variables in the shader
		@note Some variables may be absent from a particular shader variant.
	*/
	const std::shared_ptr<ShaderVariableTable> &GetVariables() const { return variables; }

private:
	std::shared_ptr<ShaderVariableTable> variables;

	uint16_t surface_draw_state{0};
};

enum RenderableContext;

/// Get the renderable context target render pass for a surface shader .
RenderPass::Pass GetSurfaceShaderTargetPassForRenderableContext(SurfaceShader &surface_shader, RenderableContext ctx);

} // namespace hg
