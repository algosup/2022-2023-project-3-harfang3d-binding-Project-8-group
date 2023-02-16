// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <atomic>
#include <memory>

namespace hg {

/// Track a set of parallel tasks
class task_set {
public:
	friend class task_system;

	inline unsigned int task_count() const { return count; }

private:
	std::atomic<unsigned int> count;
};

} // namespace hg
