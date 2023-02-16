// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/assert.h"

namespace hg {

/// Global reference to an object, the wrapped object memory is unmanaged.
template <typename T>
class global_object {
public:
	bool is_set() const { return i; }

	inline T &get() const {
		__ASSERT_MSG__(i != nullptr, "global_object is not initialized, was hg::Init called successfully?");
		return *i;
	}

	void set(T *o) {
		__ASSERT_MSG__(i == nullptr, "global_object already initialized");
		i = o;
	}

	inline operator bool() const { return bool(i); }

private:
	T *i;
};

} // namespace hg
