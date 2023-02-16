// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/get_type_name.h"

#define DeclareTypeName(TYPE, NAME)                 \
	template <>                                     \
	inline const std::string &GetTypeName<TYPE>() { \
		static std::string name(NAME);              \
		return name;                                \
	}
