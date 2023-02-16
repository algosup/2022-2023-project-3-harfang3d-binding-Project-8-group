// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

namespace hg {

template <class T>
struct range {
	range(const T &s, const T &e) : start(s), end(e) {}

	inline T value_range() const { return end - start; }

	inline bool in(const T &v) const { return start < end ? v >= start && v < end : v >= end && v < start; }

	inline void sort() {
		if (start > end) {
			T t = start;
			start = end;
			end = t;
		}
	}

	static range compute_intersection(const range<T> &a, const range<T> &b) {
		if (a.end < b.start || a.start > b.end)
			return range<T>();
		return range(a.start > b.start ? a.start : b.start, a.end < b.end ? a.end : b.end);
	}

	static range compute_union(const range &a, const range &b) { return range(a.start < b.start ? a.start : b.start, a.end > b.end ? a.end : b.end); }

	void set(const T &s, const T &e) {
		start = s;
		end = e;
	}

	template <class N>
	range<N> convert() const { return range<N>(N(start), N(end)); }

	T start = 0, end = 0;
};

} // namespace hg
