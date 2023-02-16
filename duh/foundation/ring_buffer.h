// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <vector>

namespace hg {

/// Circular ring buffer
template <class T>
class RingBuffer {
public:
	/// Get the maximum number of element in buffer.
	inline size_t GetSize() const { return buffer.size(); }

	/// Number of object left to put in the buffer.
	inline size_t GetFree() const { return buffer.size() - usage; }
	/// Number of object available to get from the buffer.
	inline size_t GetUsage() const { return usage; }

	/// Increment the put pointer, returns true when successful.
	bool Produce() {
		if (usage == buffer.size())
			return false;

		put = (put + 1) % buffer.size();
		usage++;

		return true;
	}

	/// Increment the get pointer, returns true when successful.
	bool Consume() {
		if (!usage)
			return false;

		get = (get + 1) % buffer.size();
		usage--;

		return true;
	}

	/// Retrieve the object at the current get pointer.
	inline T &CurrentGet() { return buffer[get]; }
	/// Retrieve the object at the current put pointer.
	inline T &CurrentPut() { return buffer[put]; }

	bool Allocate(size_t count) {
		put = get = usage = 0;
		return buffer.resize(count);
	}

private:
	std::vector<T> buffer;
	size_t put{0}, get{0}, usage{0};
};

} // namespace hg
