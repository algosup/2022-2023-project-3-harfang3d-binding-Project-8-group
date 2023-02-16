// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/audio_data.h"
#include "engine/sound.h"
#include "foundation/cext.h"
#include "foundation/matrix4.h"
#include "foundation/task_system.h"
#include "foundation/timer.h"
#include "foundation/vector3.h"

namespace hg {

enum MixerLoopMode : uint8_t {
	MixerNoLoop,
	MixerRepeat,
	MixerLoopInvalidChannel
};

enum MixerPlayState : uint8_t {
	MixerStopped,
	MixerPlaying,
	MixerPaused,
	MixerStateInvalidChannel
};

typedef int32_t MixerChannel;
typedef uint8_t MixerPriority;

struct MixerChannelState {
	MixerChannelState() = default;
	MixerChannelState(float volume_) : volume(volume_) {}
	MixerChannelState(float volume_, bool direct_) : volume(volume_), direct(direct_) {}
	MixerChannelState(MixerPriority priority_, float volume_ = 1, MixerLoopMode loop_mode_ = MixerNoLoop, float pitch_ = 1, bool direct_ = false)
		: priority(priority_), loop_mode(loop_mode_), volume(volume_), pitch(pitch_), direct(direct_) {}

	MixerPriority priority{63};
	MixerLoopMode loop_mode{MixerNoLoop};

	float volume{1};
	float pitch{1};

	bool direct{false}; // disable all realtime spatialization on this channel
};

struct MixerChannelLocation {
	MixerChannelLocation() = default;
	MixerChannelLocation(const Vector3 &p) : position(p) {}

	Vector3 position{0, 0, 0};
	Vector3 velocity{0, 0, 0};
};

/// Mixer interface
struct Mixer {
	static const MixerChannelState DefaultState, RepeatState;
	static const MixerChannelLocation DefaultLocation;

	static const MixerPriority DefaultPriority;
	static const MixerChannel ChannelError;

	Mixer();
	virtual ~Mixer();

	task_affinity GetWorkerAffinity() const { return worker_affinity; }
	void SetWorkerAffinity(task_affinity affinity) { worker_affinity = affinity; }

	virtual bool Open() = 0;
	virtual void Close() = 0;

	virtual float GetMasterVolume() = 0;
	virtual void SetMasterVolume(float volume) = 0;

	virtual bool EnableSpatialization(bool enable) = 0;

	/// Play a sound, returns the channel on which the stream was started.
	virtual MixerChannel Start(Sound &sound, MixerChannelLocation location, MixerChannelState state) = 0;
	MixerChannel Start(Sound &sound, MixerChannelLocation location) { return Start(sound, location, DefaultState); }
	MixerChannel Start(Sound &sound, MixerChannelState state) { return Start(sound, DefaultLocation, state); }
	MixerChannel Start(Sound &sound) { return Start(sound, DefaultLocation, DefaultState); }
	/// Start a stream, returns the channel on which the str1eam was started.
	virtual MixerChannel StreamData(std::shared_ptr<AudioData> data, MixerChannelLocation location, MixerChannelState state, bool paused = false, time_ns t_start = 0) = 0;
	MixerChannel StreamData(std::shared_ptr<AudioData> data, MixerChannelLocation location, bool paused = false, time_ns t_start = 0) { return StreamData(data, location, DefaultState, paused, t_start); }
	MixerChannel StreamData(std::shared_ptr<AudioData> data, MixerChannelState state, bool paused = false, time_ns t_start = 0) { return StreamData(data, DefaultLocation, state, paused, t_start); }
	MixerChannel StreamData(std::shared_ptr<AudioData> data, bool paused = false, time_ns t_start = 0) { return StreamData(data, DefaultLocation, DefaultState, paused, t_start); }

	virtual MixerPlayState GetPlayState(MixerChannel channel) = 0;

	virtual MixerChannelState GetChannelState(MixerChannel channel) = 0;
	virtual void SetChannelState(MixerChannel channel, MixerChannelState state) = 0;

	virtual MixerChannelLocation GetChannelLocation(MixerChannel channel) = 0;
	virtual void SetChannelLocation(MixerChannel channel, MixerChannelLocation location) = 0;

	/// Return the channel timestamp in the currently played back media.
	virtual time_ns GetChannelTimestamp(MixerChannel channel) = 0;

	virtual void Stop(MixerChannel channel) = 0;
	virtual void Pause(MixerChannel channel) = 0;
	virtual void Resume(MixerChannel channel) = 0;
	virtual void StopAll() = 0;

	virtual void SetStreamLoopPoint(MixerChannel channel, time_ns t) = 0;
	virtual void SeekStream(MixerChannel channel, time_ns t) = 0;
	/// Return the percentage of buffers available [0;100] for the stream running on a channel. Returns -1 if no stream is playing on the channel.
	virtual int GetStreamBufferingPercentage(MixerChannel channel) = 0;

	/// Set the transformation on a stream audio data.
	virtual void SetChannelStreamDataTransform(MixerChannel channel, const Matrix4 &transform) = 0;
	virtual void FlushChannelBuffers(MixerChannel channel) = 0;

	virtual Matrix4 GetListener() = 0;
	virtual void SetListener(const Matrix4 &transform) = 0;

	virtual std::shared_ptr<Sound> LoadSoundData(std::shared_ptr<AudioData> data, const std::string &path = std::string()) = 0;
	virtual void FreeSound(Sound &sound) = 0;

	/// Start streaming an audio file, returns the channel on which the stream was started.
	virtual MixerChannel Stream(const std::string &path, MixerChannelLocation location, MixerChannelState state, bool paused = false, time_ns t_start = 0);
	MixerChannel Stream(const std::string &path, MixerChannelLocation location, bool paused = false, time_ns t_start = 0) { return Stream(path, location, DefaultState, paused, t_start); }
	MixerChannel Stream(const std::string &path, MixerChannelState state, bool paused = false, time_ns t_start = 0) { return Stream(path, DefaultLocation, state, paused, t_start); }
	MixerChannel Stream(const std::string &path, bool paused = false, time_ns t_start = 0) { return Stream(path, DefaultLocation, DefaultState, paused, t_start); }
	/// Load a sound from an audio file.
	virtual std::shared_ptr<Sound> LoadSound(const std::string &path);

protected:
	task_affinity worker_affinity;
	timer_thread::handle update_timer;

	friend void PeriodicMixerUpdateCallback(Mixer *mixer);

	virtual void Update() = 0;
};

/// Create a periodic timer task to call the specified mixer Update method.
timer_thread::handle StartMixerUpdateTimer(Mixer *mixer);
/// Cancel a mixer Update periodic timer task.
void CancelMixerUpdateTimer(timer_thread::handle mixer_handle);

} // namespace hg
