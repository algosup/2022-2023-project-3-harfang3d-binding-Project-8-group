// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/shader.h"
#include <memory>

namespace hg {

struct MaterialValue : ShaderValue {
	std::string name;
	ShaderType type{ShaderNoType};
};

bool operator==(const MaterialValue &a, const MaterialValue &b);

/**
	@short Material.
	A material associates a shader and a set of values for the shader inputs.
*/
struct Material {
	Material() { values.reserve(8); }

	std::string name, shader;
	std::vector<MaterialValue> values;

	MaterialValue *GetValue(const std::string &name);
	MaterialValue *AddValue(const std::string &name, ShaderType type);
};

bool operator==(const Material &a, const Material &b);

} // namespace hg
