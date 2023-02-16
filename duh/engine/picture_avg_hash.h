// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#include "foundation/math.h"
#include <cstdint>

namespace hg {

class Picture;

struct PictureAvgHash {
	/// Return true if the number of differences between two hashes is below threshold.
	bool IsEquivalent(const PictureAvgHash &hash, int threshold = 0) const;

	uint32_t v[2];
};

/// Return a picture average hash. Such a hash can be used to perform perceptual comparison.
PictureAvgHash ComputePictureAvgHash(const Picture &picture);

} // namespace hg
