// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/vector2.h"

namespace hg {

struct Texture;

void InstallDebugger();
void UninstallDebugger();

bool GetEnableDebugger();
void SetEnableDebugger(bool enable);

void DebuggerPauseScene(bool pause);

void DrawTaskSystemDebugger(bool *debug = nullptr);
void DrawFrameProfiler(bool *debug = nullptr);

Vector2 GetTextureDisplayRect(Texture &tex, float size);
void TextureTooltip(Texture &tex);

} // namespace hg
