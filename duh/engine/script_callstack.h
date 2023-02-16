// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/reflection.h"
#include <string>
#include <vector>

namespace hg {

struct ScriptCallstackLocal {
	std::string name;
	TypeValue value;
};

struct ScriptCallstackFrame {
	std::string location; // location string formatted with as much information as possible (dependent on the VM)
	std::string file;
	size_t line{0};
	std::vector<ScriptCallstackLocal> locals;
};
	
struct ScriptCallstack {
	std::vector<ScriptCallstackFrame> frames;
};

} // namespace hg
