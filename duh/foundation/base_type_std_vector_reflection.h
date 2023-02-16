// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/reflection.h"
#include "foundation/declare_type_name.h"

namespace hg {

void RegisterBaseTypeStdVectorReflection(TypeRegistry &registry);

DeclareTypeName(std::vector<bool>, "BoolVector");
DeclareTypeName(std::vector<char>, "CharVector");
DeclareTypeName(std::vector<short>, "ShortVector");
DeclareTypeName(std::vector<int>, "IntVector");
DeclareTypeName(std::vector<unsigned char>, "UnsignedCharVector");
DeclareTypeName(std::vector<unsigned short>, "UnsignedShortVector");
DeclareTypeName(std::vector<unsigned int>, "UnsignedIntVector");
DeclareTypeName(std::vector<float>, "FloatVector");
DeclareTypeName(std::vector<std::string>, "StringVector");

} // namespace hg
