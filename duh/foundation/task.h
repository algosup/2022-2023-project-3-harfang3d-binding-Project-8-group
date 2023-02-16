// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/task_set.h"
#include "foundation/time.h"

namespace hg {

typedef char task_affinity;
typedef char task_priority;

static const task_affinity any_task_worker = -1;
static const task_priority default_task_priority = 0;

class task {
public:
	friend class task_system;

	virtual ~task() {}

	virtual void run() = 0;
	virtual const char *GetName() const; // WARNING the returned buffer is shared by all tasks, do not call GetName() before storing the result of a previous call

	const std::shared_ptr<task_set> &get_task_set() const { return set; }

private:
	std::shared_ptr<task_set> set;
	task_affinity aff{any_task_worker};
	task_priority prt{default_task_priority};
	size_t t_queued{0};
	size_t t_raised{0};
};

struct task_info {
	std::string name;
	task_priority prt{default_task_priority};
	size_t t_queued{0};
	size_t t_raised{0};
};

} // namespace hg
