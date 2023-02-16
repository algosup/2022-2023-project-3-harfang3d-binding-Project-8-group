// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/picture_io_codec.h"
#include "foundation/global_object.h"
#include <string>
#include <vector>

namespace hg {

/// Picture I/O class
class PictureIO {
public:
	/// Load a picture.
	bool Load(Picture &picture, const std::string &path, const std::string &codec_name = std::string());
	/// Save a picture through a specific codec.
	bool Save(const Picture &picture, const std::string &path, const std::string &codec_name, const std::string &parameters = std::string());

	/// Return a list of the available picture codecs.
	const std::vector<std::unique_ptr<PictureCodec>> &GetCodecList() const { return codecs; }
	/// Return a codec from its name.
	PictureCodec *Codec(const std::string &codec_name);

	/// Register an I/O codec inside the manager.
	bool RegisterCodec(PictureCodec *codec, bool verbose = false);
	/// Delete all registered codec.
	void DeleteCodecs() { codecs.clear(); }

	/// Return a comma separated list of supported extensions.
	std::vector<std::string> GetSupportedExt() const;

private:
	std::vector<std::unique_ptr<PictureCodec>> codecs;
};

extern global_object<PictureIO> g_picture_io;

} // namespace hg
