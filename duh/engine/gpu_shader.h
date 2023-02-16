// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/shader.h"
#include "engine/texture.h"
#include "foundation/declare_type_name.h"
#include "foundation/math.h"
#include <array>

namespace hg {

struct GpuShaderValue;

/// GPU shader.
struct GpuShader : public GpuResource {
	explicit GpuShader(const std::string &name) : GpuResource(name) {}
	GpuShader(const GpuShader &shader) = delete;

	virtual bool UsesBuiltin(ShaderVariable::Semantic semantic) const = 0;

	std::shared_ptr<ShaderVariableTable> variable_table;
	std::vector<GpuShaderValue> values;

	GpuShaderValue *GetValue(const std::string &name);
};

struct GpuShaderValue {
	union {
		std::array<int, 16> iv{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
		std::array<uint32_t, 16> uv;
		std::array<float, 16> fv;
	};

	std::shared_ptr<Texture> texture;
	TextureUnitConfig tex_unit_cfg;
};

GpuShaderValue CoreShaderValueToGpuShaderValue(Renderer &renderer, ShaderType type, const ShaderValue &in);

void GpuShaderDeleter(GpuShader *shader, std::weak_ptr<Renderer> renderer_weak, task_affinity worker);

DeclareTypeName(GpuShader, "GpuShader");
DeclareTypeName(std::shared_ptr<GpuShader>, "std::shared_ptr<GpuShader>");

} // namespace hg
