// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <memory>
#include <string>

namespace hg {

class AudioData;

/// Audio codec base class.
struct AudioCodec {
	virtual ~AudioCodec() {}

	/// Open audio data.
	virtual std::shared_ptr<AudioData> Open(const std::string &path) = 0;

	/// Return a comma separated list of supported extensions.
	virtual std::string GetSupportedExt() const = 0;
};

} // namespace hg
