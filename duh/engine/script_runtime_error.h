// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/script_callstack.h"
#include <memory>
#include <string>

namespace hg {

class VM;
class ScriptExecutionContext;

struct ScriptRuntimeError {
	std::string msg;

	std::shared_ptr<VM> vm;
	std::shared_ptr<ScriptExecutionContext> execution_context;

	ScriptCallstack callstack;
};

} // namespace hg
