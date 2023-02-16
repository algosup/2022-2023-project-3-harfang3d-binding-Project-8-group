// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "imgui.h"
#include "foundation/time.h"

namespace ImGui {

// obsolete functions
static inline void SetNextWindowContentWidth(float width) { SetNextWindowContentSize(ImVec2(width, 0.0f)); } // OBSOLETE 1.53+ (nb: original version preserved last Y value set by SetNextWindowContentSize())
static inline void SetNextWindowPosCenter(ImGuiCond cond = 0) { SetNextWindowPos(ImVec2(GetIO().DisplaySize.x * 0.5f, GetIO().DisplaySize.y * 0.5f), cond, ImVec2(0.5f, 0.5f)); } // OBSOLETE 1.52+

bool time_ns_Edit(const char *label, hg::time_ns &v);

} // namespace ImGui
