// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstddef>

namespace hg {

/// Point 2D
template <class T>
struct Point {
	Point() = default;
	Point(T ux, T uy) : x(ux), y(uy) {}

	T operator[](size_t n) const { return *(&x + n); }

	void Set(T _x, T _y) {
		x = _x;
		y = _y;
	}

	T x{0}, y{0};
};

typedef Point<int> iPoint;
typedef Point<float> fPoint;

} // namespace hg
