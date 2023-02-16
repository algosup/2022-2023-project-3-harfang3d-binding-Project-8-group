// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once
#include <string>

struct lua_State;

namespace hg {

class TypeValue;

bool PushTypeValueToLua(lua_State *L, const TypeValue &val);
bool GetTypeValueFromLua(lua_State *L, TypeValue &out, int index, const std::string &cast_to);

} // namespace hg
