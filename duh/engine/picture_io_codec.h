// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace hg {

enum PictureCodecCaps {
	PictureCodecCanRead = 0x01, ///< Codec can read data.
	PictureCodecCanWrite = 0x02, ///< Codec can write data.
	PictureCodecWriteRaw = 0x04, ///< Codec supports raw output.
	PictureCodecWriteLossy = 0x08, ///< Codec supports lossy compression.
	PictureCodecWriteLossless = 0x10, ///< Codec supports lossless compression.
	PictureCodecAlphaChannel = 0x20, ///< Codec supports alpha channel.
	PictureCodecRealData = 0x40 ///< Codec supports real data.
};

class FileHandle;
class Picture;

/// Picture I/O codec
struct PictureCodec {
	virtual ~PictureCodec() {}

	virtual bool Load(FileHandle &handle, Picture &picture) = 0;
	virtual bool Save(FileHandle &handle, const Picture &picture, const std::string &parm = std::string()) { return false; }

	virtual std::string GetName() const = 0;

	virtual PictureCodecCaps GetCaps() const = 0;
	virtual std::vector<std::string> GetSupportedExt() const = 0;
};

} // namespace hg
