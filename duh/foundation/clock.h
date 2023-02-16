// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/smart_median_average.h"
#include "foundation/time.h"

namespace hg {

/// Clock object tracking elapsed time delta
class Clock {
public:
	Clock();

	void Reset();

	void Update();
	void Skip();

	inline time_ns Get() const { return current; }
	inline time_ns GetDelta() const { return dt; }

private:
	time_ns last{0}, current{0};
	time_ns dt{0};

	SmartMedianAverage<time_ns> dt_history;
};

} // namespace hg
