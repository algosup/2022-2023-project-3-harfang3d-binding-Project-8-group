// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/global_object.h"
#include "foundation/rect.h"
#include "foundation/signal.h"
#include "foundation/string.h"
#include <array>
#include <map>

namespace hg {

struct Monitor {
	std::array<int8_t, 64> data{};
}; // 64 bytes

enum MonitorRotation : uint8_t {
	Rotation_0 = 1,
	Rotation_90 = 2,
	Rotation_180 = 4,
	Rotation_270 = 8
};

struct MonitorMode {
	std::string name;
	hg::iRect rect;
	int frequency;
	MonitorRotation rotation;
	uint8_t supported_rotations;
};

/// Return the monitor rect.
iRect GetMonitorRect(const Monitor &monitor);
/// Return true if the monitor is set as primary.
bool IsPrimaryMonitor(const Monitor &monitor);
/// Return true if the monitor is connected.
bool IsMonitorConnected(const Monitor &monitor);
/// Return monitor name.
std::string GetMonitorName(const Monitor &monitor);
/// Return monitor size in millimeters.
hg::iVector2 GetMonitorSizeMM(const Monitor &monitor);
/// Return monitor content scale.
hg::Vector2 GetMonitorContentScale(const Monitor &monitor);
/// Get the list of screen modes for a given monitor.
bool GetMonitorModes(const Monitor& monitor, std::vector<MonitorMode> &modes);

//
struct Surface {
	std::array<int8_t, 32> data{};
}; // 32 bytes

extern Surface NoSurface;

bool operator==(const Surface &a, const Surface &b);

//
struct Window {
	enum Visibility {
		Windowed,
		Undecorated,
		Fullscreen,
		Hidden,
		FullscreenMonitor1,
		FullscreenMonitor2,
		FullscreenMonitor3
	};

	std::array<int8_t, 32> data{};
}; // 32 bytes

bool operator==(const Window &a, const Window &b);

/// Must be called from the main thread.
void WindowSystemInit();

/// Create a new window.
Window NewWindow(int width, int height, int bpp = 32, Window::Visibility visibility = Window::Windowed);
/// Wrap a foreign window native handle in a new window.
Window NewWindowFrom(void *handle);
/// Create a new fullscreen window on a specified monitor.
Window NewFullscreenWindow(const Monitor& monitor, int mode_index, MonitorRotation rotation = Rotation_0);

/// Return the window native handle.
void *GetWindowHandle(const Window &w);

/// Pool window events.
bool UpdateWindow(const Window &w);

/// Destroy a window.
bool DestroyWindow(Window &w);

/// Get the window client size.
bool GetWindowClientSize(const Window &w, int &width, int &height);
/// Set the window client size.
bool SetWindowClientSize(const Window &w, int width, int height);

/// Get the window content scale.
bool GetWindowContentScale(const Window &w, float &xscale, float &yscale);

/// Get the window title as an UTF-8 string.
bool GetWindowTitle(const Window &w, std::string &title);
/// Set the window title from an UTF-8 string.
bool SetWindowTitle(const Window &w, const std::string &title);

/// Return true if the provided window has input focus.
bool WindowHasFocus(const Window &w);

/// Return the system handle to the window currently in focus.
Window GetWindowInFocus();

/// Get a list of monitors connected to the system.
std::vector<Monitor> GetMonitors();

/// Get the window position
iVector2 GetWindowPos(const Window &w);
/// Set the window position
bool SetWindowPos(const Window &w, const iVector2 &v);

/// Return true if the window is open.
bool IsWindowOpen(const Window &w);

/// Window system shared state.
struct WindowSystem {
	Window window_in_focus;

	Signal<void(const Window &)> new_window_signal;
	Signal<void(const Window &, bool)> window_focus_signal;
	Signal<bool(const Window &)> close_window_signal;
	Signal<void(const Window &)> destroy_window_signal;

    void *impl {nullptr};

#if ANDROID
	void *display = nullptr;
	void *main_window_handle = nullptr;
#endif
};

extern global_object<WindowSystem> g_window_system;

void ConnectWindowSystemSignals();
void DisconnectWindowSystemSignals();

void ShowCursor();
void HideCursor();

} // namespace hg
