// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/global_object.h"
#include "foundation/signal.h"
#include "foundation/task.h"
#include "foundation/time.h"
#include <atomic>

/// Harfang API
namespace hg {

class Renderer;

/// Core engine signals and properties
struct Engine {
	Signal<void(Renderer &)> new_renderer_signal, delete_renderer_signal;

	Signal<void()> end_frame_signal; // end of frame signaled
	std::atomic<uint64_t> last_frame_duration{0}, last_frame_end{0};

	Signal<void()> app_pause_signal; // request that the application be paused
	Signal<void()> app_resume_signal; // request that the application be resumed
};

extern global_object<Engine> g_engine;

/// Mark the end of the current frame.
void EndFrame();
/// Get the current frame timestamp, start of current frame, end of last frame.
time_ns GetCurrentFrameTimestamp();
/// Get the last frame duration.
time_ns GetLastFrameDuration();
/// Get the last frame duration in seconds.
float GetLastFrameDurationSec();
/// Reset the last frame duration.
void ResetLastFrameDuration();

/// Helper to break into the native debugger from a known script position.
void _DebugHalt();

//
static constexpr task_affinity renderer_task_affinity = 0;
static constexpr task_affinity general_task_affinity = task_affinity(0xfe);

} // namespace hg
