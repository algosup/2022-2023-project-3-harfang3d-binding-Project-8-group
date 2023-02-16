// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstdint>
#include <cstddef>

namespace hg {

/// Audio format
struct AudioFormat {
	enum Encoding : uint8_t {
		PCM,
		WiiADPCM
	};

	enum Type : uint8_t {
		Integer,
		Float
	};

	AudioFormat(Encoding enc = PCM, uint8_t ch = 2, uint32_t fq = 48000, uint8_t res = 16, Type tp = Integer) : encoding(enc), channels(ch), frequency(fq), resolution(res), type(tp) {}

	Encoding encoding;
	uint8_t channels;

	uint32_t frequency;
	uint8_t resolution;

	Type type;
};

/// Get the memory usage of a number of audio sample.
inline size_t GetAudioSampleDataSize(uint32_t sample_count, int channels, uint8_t resolution) { return (sample_count * channels * resolution) / 8; }

} // namespace hg
