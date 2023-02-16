// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "foundation/time.h"

namespace hg {

class Renderer;

struct Mixer;
struct Texture;

/// Movie interface
class Movie {
public:
	enum FrameFormat {
		YUV3PlanesHalfChroma, //< 3 separate planes with the U and V planes of half resolution
		ExternalOES, //< external OES texture (OpenGL ES specific extension)
		YUV422
	};

	virtual ~Movie() {}

	virtual bool Open(std::shared_ptr<Renderer> renderer, std::shared_ptr<Mixer> mixer, const std::string &path, bool paused = false) = 0;
	virtual bool Play() = 0;
	virtual bool Pause() = 0;
	virtual bool Close() = 0;

	bool Open(std::shared_ptr<Renderer> renderer, const std::string &path, bool paused = false) { return Open(renderer, nullptr, path, paused); }

	virtual time_ns GetDuration() const = 0;
	virtual time_ns GetTimeStamp() const = 0;
	virtual bool Seek(time_ns t) = 0;
	virtual bool IsEnded() const = 0;

	virtual FrameFormat GetFormat() const = 0;

	/**
		Returns texture refs holding the movie frame for the current clock. Textures returned by
		previous calls to this function are not valid after this function returns. The textures should
	 	be presented to the viewer as soon as possible to prevent loss of A/V synchronization.
	 */
	virtual bool GetFrame(std::vector<std::shared_ptr<Texture>> &planes) = 0;
};

} // namespace hg
