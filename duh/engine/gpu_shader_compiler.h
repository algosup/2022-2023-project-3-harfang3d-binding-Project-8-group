// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <memory>
#include <string>

namespace hg {

class Renderer;

struct GpuShader;
struct ShaderVariableTable;

/// GPU shader compiler interface
struct GpuShaderCompiler {
	virtual ~GpuShaderCompiler() {}
	virtual bool Compile(Renderer &renderer, std::shared_ptr<GpuShader> shader, std::shared_ptr<ShaderVariableTable> var_table, const std::string &vertex_source, const std::string &pixel_source) = 0;
};

} // namespace hg
