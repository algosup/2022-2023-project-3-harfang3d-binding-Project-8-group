// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/global_object.h"
#include "engine/audio_codec.h"
#include <map>
#include <memory>
#include <vector>

namespace hg {

class AudioData;

/// Audio factory
class AudioIO {
public:
	/// Register an audio codec.
	void RegisterCodec(const std::string &name, std::unique_ptr<AudioCodec> codec);
	/// Open audio data from a path.
	std::shared_ptr<AudioData> Open(const std::string &path);
	std::shared_ptr<AudioData> Open(const std::string &path, const std::string &codec);

	/// Return a comma separated list of supported extensions.
	std::string GetSupportedExt() const;

private:
	std::map<std::string, std::unique_ptr<AudioCodec>> codecs;
};

extern global_object<AudioIO> g_audio_io;

} // namespace hg
