// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/declare_type_name.h"
#include "foundation/reflection.h"
#include <string>

namespace hg {

void RegisterBaseTypeReflection(TypeRegistry &registry);

struct none {}; ///< Represents the none type as understood by most script VM in the reflection system.

struct func { ///< A script function.
	std::string info;
};

struct cfunc { ///< A C function as held by the VM.
	std::string info;
};

DeclareTypeName(bool, "Bool");
DeclareTypeName(char, "Char");
DeclareTypeName(short, "Short");
DeclareTypeName(int, "Int");
DeclareTypeName(unsigned char, "UnsignedChar");
DeclareTypeName(unsigned short, "UnsignedShort");
DeclareTypeName(unsigned int, "UnsignedInt");
#if MARCH_WORD_SIZE > 32 // on 32-bit architectures size_t is a typedef to a 32-bit int
DeclareTypeName(size_t, "SizeT");
#endif
DeclareTypeName(float, "Float");
DeclareTypeName(std::string, "String");
DeclareTypeName(none, "None");
DeclareTypeName(func, "Function");
DeclareTypeName(cfunc, "CFunction");

} // namespace hg
