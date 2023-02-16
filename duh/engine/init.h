// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/signal.h"
#include "foundation/time.h"
#include <atomic>

namespace hg {

extern Signal<void()> post_init_signal; //< post_init_signal is used before g_engine is initialized

/// Return the executable path.
const std::string &GetExecutablePath();

/// Initialize framework.
void Init(const std::string &executable_path = std::string());
/// Uninitialize framework.
void Uninit();

} // namespace hg
