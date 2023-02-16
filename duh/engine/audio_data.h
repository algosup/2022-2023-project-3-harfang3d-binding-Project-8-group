// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/audio_format.h"
#include "foundation/time.h"
#include <memory>
#include <vector>

namespace hg {

class Matrix4;

/// Audio data base class
class AudioData {
public:
	enum State {
		Ready,
		Ended,
		Disconnected
	};

	virtual ~AudioData() = default;

	/// Return the format under which data is read from this source.
	virtual AudioFormat GetFormat() const = 0;

	/// Open data source.
	virtual bool Open(const std::string &path) = 0;
	/// Close data source.
	virtual void Close() = 0;

	/// Return the state of the data source.
	virtual State GetState() const = 0;

	/// Seek in data source.
	virtual bool Seek(time_ns t) = 0;

	/// Get the next frame of interleaved PCM data, return the frame size.
	virtual size_t GetFrame(void *data, time_ns &frame_t) = 0;
	/// Return the maximum size of a frame of PCM data.
	virtual size_t GetFrameSize() const = 0;

	/// Set the audio data transformation matrix.
	virtual void SetTransform(const Matrix4 &mtx);

	/*
		@short Return the total audio data size.

		Some sources might not support this method and will need frame pulling
		to completely extract their data.

		@note Some source might return an estimated data size. The caller is
		responsible for handling the case where more data is returned by calling
		GetFrame() successively until the end of the source than was returned by
		this function.
	*/
	virtual size_t GetDataSize() const { return 0; }
};

/// Extract all data from an audio data source.
size_t ExtractAudioData(AudioData &source, std::vector<char> &data);

} // namespace hg
