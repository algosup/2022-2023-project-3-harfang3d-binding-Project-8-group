// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/math.h"
#include <string>
#include <vector>

namespace hg {

/// Native C++ callstack frame
struct CallStackFrame {
	void *address{nullptr};

	std::string function; // function name
	std::string source; // source path

	uint32_t line{0};
};

/// Native C++ callstack capture
struct CallStack {
	std::vector<CallStackFrame> frames;
};

/// Capture the current C++ callstack
void CaptureCallstack(CallStack &callstack, uint32_t skip_frames = 0, void *context = nullptr);

} // namespace hg

namespace std {

std::string to_string(const hg::CallStack &callstack);

} // namespace std

