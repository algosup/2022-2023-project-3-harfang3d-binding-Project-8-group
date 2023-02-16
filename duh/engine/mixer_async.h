// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/mixer.h"
#include "foundation/call_task.h"
#include "foundation/cext.h"
#include "foundation/timer.h"

namespace hg {

#define MA_task_affinity mixer->GetWorkerAffinity()

/// Mixer async interface
class MixerAsync {
public:
	MixerAsync(std::shared_ptr<Mixer> mixer_, task_affinity affinity = 1) : mixer(mixer_) { mixer->SetWorkerAffinity(affinity); }

	const std::shared_ptr<Mixer> &GetMixer() const { return mixer; }

	std::future<bool> Open() { return run_call_with_return_value<bool>(std::bind(&Mixer::Open, shared_ref(mixer)), MA_task_affinity); }
	std::future<void> Close() { return run_call_with_return_value<void>(std::bind(&Mixer::Close, shared_ref(mixer)), MA_task_affinity); }

	std::future<bool> EnableSpatialization(bool enable) { return run_call_with_return_value<bool>(std::bind(&Mixer::EnableSpatialization, shared_ref(mixer), enable), MA_task_affinity); }

	std::future<float> GetMasterVolume() { return run_call_with_return_value<float>(std::bind(&Mixer::GetMasterVolume, shared_ref(mixer)), MA_task_affinity); }
	void SetMasterVolume(float volume) { run_call<void>(std::bind(&Mixer::SetMasterVolume, shared_ref(mixer), volume), MA_task_affinity); }

	std::future<MixerChannel> Start(std::shared_ptr<Sound> sound, MixerChannelLocation location, MixerChannelState state) { return run_call_with_return_value<MixerChannel>(std::bind(&MixerAsync::Start_adapter_0, this, sound, location, state), MA_task_affinity); }
	std::future<MixerChannel> Start(std::shared_ptr<Sound> sound, MixerChannelLocation location) { return Start(sound, Mixer::DefaultLocation, Mixer::DefaultState); }
	std::future<MixerChannel> Start(std::shared_ptr<Sound> sound, MixerChannelState state) { return Start(sound, Mixer::DefaultLocation, state); }
	std::future<MixerChannel> Start(std::shared_ptr<Sound> sound) { return Start(sound, Mixer::DefaultLocation, Mixer::DefaultState); }

	std::future<MixerChannel> Stream(const std::string &path, MixerChannelLocation location, MixerChannelState state, bool paused = false, time_ns t_start = 0) { return run_call_with_return_value<MixerChannel>(std::bind(&MixerAsync::Stream_adapter_0, this, path, location, state, paused, t_start), MA_task_affinity); }
	std::future<MixerChannel> Stream(const std::string &path, MixerChannelLocation location, bool paused = false, time_ns t_start = 0) { return Stream(path, location, Mixer::DefaultState, paused, t_start); }
	std::future<MixerChannel> Stream(const std::string &path, MixerChannelState state, bool paused = false, time_ns t_start = 0) { return Stream(path, Mixer::DefaultLocation, state, paused, t_start); }
	std::future<MixerChannel> Stream(const std::string &path, bool paused = false, time_ns t_start = 0) { return Stream(path, Mixer::DefaultLocation, Mixer::DefaultState, paused, t_start); }

	std::future<MixerPlayState> GetPlayState(MixerChannel channel) { return run_call_with_return_value<MixerPlayState>(std::bind(&Mixer::GetPlayState, shared_ref(mixer), channel), MA_task_affinity); }

	std::future<MixerChannelState> GetChannelState(MixerChannel channel) { return run_call_with_return_value<MixerChannelState>(std::bind(&Mixer::GetChannelState, shared_ref(mixer), channel), MA_task_affinity); }
	void SetChannelState(MixerChannel channel, MixerChannelState state) { run_call<void>(std::bind(&Mixer::SetChannelState, shared_ref(mixer), channel, state), MA_task_affinity); }

	std::future<MixerChannelLocation> GetChannelLocation(MixerChannel channel) { return run_call_with_return_value<MixerChannelLocation>(std::bind(&Mixer::GetChannelLocation, shared_ref(mixer), channel), MA_task_affinity); }
	void SetChannelLocation(MixerChannel channel, MixerChannelLocation location) { run_call<void>(std::bind(&Mixer::SetChannelLocation, shared_ref(mixer), channel, location), MA_task_affinity); }

	std::future<time_ns> GetChannelTimestamp(MixerChannel channel) { return run_call_with_return_value<time_ns>(std::bind(&Mixer::GetChannelTimestamp, shared_ref(mixer), channel), MA_task_affinity); }

	void Stop(MixerChannel channel) { run_call<void>(std::bind(&Mixer::Stop, shared_ref(mixer), channel), MA_task_affinity); }
	void Pause(MixerChannel channel) { run_call<void>(std::bind(&Mixer::Pause, shared_ref(mixer), channel), MA_task_affinity); }
	void Resume(MixerChannel channel) { run_call<void>(std::bind(&Mixer::Resume, shared_ref(mixer), channel), MA_task_affinity); }
	void StopAll() { run_call<void>(std::bind(&Mixer::StopAll, shared_ref(mixer)), MA_task_affinity); }

	void SetStreamLoopPoint(MixerChannel channel, uint64_t t_ns) { run_call<void>(std::bind(&Mixer::SetStreamLoopPoint, shared_ref(mixer), channel, t_ns), MA_task_affinity); }
	void SeekStream(MixerChannel channel, uint64_t t_ns) { run_call<void>(std::bind(&Mixer::SeekStream, shared_ref(mixer), channel, t_ns), MA_task_affinity); }
	std::future<int> GetStreamBufferingPercentage(MixerChannel channel) { return run_call_with_return_value<int>(std::bind(&Mixer::GetStreamBufferingPercentage, shared_ref(mixer), channel), MA_task_affinity); }

	void SetChannelStreamDataTransform(MixerChannel channel, const Matrix4 &transform) { run_call<void>(std::bind(&Mixer::SetChannelStreamDataTransform, shared_ref(mixer), channel, transform), MA_task_affinity); }
	void FlushChannelBuffers(MixerChannel channel) { run_call<void>(std::bind(&Mixer::FlushChannelBuffers, shared_ref(mixer), channel), MA_task_affinity); }

	std::future<Matrix4> GetListener() { return run_call_with_return_value<Matrix4>(std::bind(&Mixer::GetListener, shared_ref(mixer)), MA_task_affinity); }
	void SetListener(const Matrix4 &world_mat) { run_call<void>(std::bind(&Mixer::SetListener, shared_ref(mixer), world_mat), MA_task_affinity); }

	std::shared_ptr<Sound> LoadSound(const std::string &path) { return mixer->LoadSound(path); }
	void FreeSound(const std::shared_ptr<Sound> &sound) { run_call<void>(std::bind(&Mixer::FreeSound, shared_ref(mixer), shared_ref(sound)), MA_task_affinity); }

private:
	MixerChannel Start_adapter_0(std::shared_ptr<Sound> sound, MixerChannelLocation location, MixerChannelState state) { return mixer->Start(*sound, location, state); }
	MixerChannel Stream_adapter_0(const std::string &path, MixerChannelLocation location, MixerChannelState state, bool paused, time_ns t_start) { return mixer->Stream(path, location, state, paused, t_start); }

	std::shared_ptr<Mixer> mixer;
};

} // namespace hg
