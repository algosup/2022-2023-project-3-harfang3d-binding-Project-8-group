// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/script_object.h"
#include <memory>
#include <string>

namespace hg {

class ScriptExecutionContext {
public:
	enum State {
		StateUp,
		StateDown
	};

	virtual ~ScriptExecutionContext() {}

	virtual std::string GetDescription() const = 0;
	virtual ScriptObject GetEnv() const = 0; // return the execution context environment object

	bool IsUp() const { return state == StateUp; }
	void MarkDown() { state = StateDown; } // no code should run on this context anymore

private:
	State state{StateUp};
};

} // namespace hg
