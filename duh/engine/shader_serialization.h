// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <string>
#include <vector>
#include <memory>

namespace hg {

enum ShaderType : uint8_t;

struct Shader;
struct ShaderValue;
struct ShaderVariable;
struct ShaderVariant;

/// Parse a shader value from string.
bool ParseShaderValue(const std::string &value, ShaderValue &out, ShaderType type, bool use_naked_path = false);
/// Encode a shader value to string.
std::string ShaderValueToString(const ShaderValue &in, ShaderType type, bool use_naked_path = false);

/// Load a shader input variables declaration.
bool LoadShaderVariables(const std::string &name, std::vector<std::unique_ptr<ShaderVariable>> &out);

/// Load a shader from source.
bool LoadShader(Shader &shader, const std::string &source);

/// Transfer varyings from a shader variant to another variant. The variant previous content is not lost.
void TransferShaderVariantVaryings(ShaderVariant &out, const ShaderVariant &in);

} // namespace hg
