// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/global_object.h"
#include "foundation/signal.h"

struct ImGuiContext;
struct ImGuiIO;

struct ImFontAtlas;
struct ImFont;

namespace hg {

class Renderer;
class RendererAsync;

void ImGuiInstall(Renderer &renderer);
void ImGuiUninstall(Renderer &renderer);

void ImGuiLock();
void ImGuiUnlock();

struct Window;
struct Surface;

void ImGuiSetOutputSurface(const Surface &surface);
void ImGuiSetOutputWindow(const Window &window, const Surface &surface);

ImFont *ImGuiAddFont(RendererAsync &renderer_async, const std::string &font_path, const float &font_size);

struct ImGuiSystem {
	Signal<void(ImGuiIO &)> imgui_io_setup;
	Signal<void(ImGuiContext *)> imgui_context_changed;
	Signal<void(ImFontAtlas *)> imgui_setup_font_atlas;
};

extern global_object<ImGuiSystem> g_imgui;

} // namespace hg
