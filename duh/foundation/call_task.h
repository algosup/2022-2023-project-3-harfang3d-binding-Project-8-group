// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/call.h"
#include "foundation/task_system.h"

namespace hg {

template <typename R>
struct future_call_task : task {
	future_call_task(std::function<R()> f_) : f(std::move(f_)) {}

	void run() override { f(); }
	const char *GetName() const override { return f.target_type().name(); }

private:
	std::function<R()> f;
};

template <typename R>
void inline run_call(task_system &system, std::function<R()> fn, const std::shared_ptr<task_set> &set, task_affinity affinity = any_task_worker, task_priority priority = default_task_priority) { system.push(std::make_shared<future_call_task<R>>(std::move(fn)), set, affinity, priority); }
template <typename R>
void inline run_call(task_system &system, std::function<R()> fn, task_affinity affinity = any_task_worker, task_priority priority = default_task_priority) { system.push(std::make_shared<future_call_task<R>>(std::move(fn)), affinity, priority); }
template <typename R>
void inline run_call(std::function<R()> fn, const std::shared_ptr<task_set> &set, task_affinity affinity = any_task_worker, task_priority priority = default_task_priority) { run_call(g_task_system.get(), std::move(fn), set, affinity, priority); }
template <typename R>
void inline run_call(std::function<R()> fn, task_affinity affinity = any_task_worker, task_priority priority = default_task_priority) { run_call(g_task_system.get(), std::move(fn), nullptr, affinity, priority); }

//
template <typename R>
struct future_call_task_with_return_value : task {
	future_call_task_with_return_value(std::function<R()> f_) : f(std::move(f_)) {}

	std::future<R> get_future() { return f.get_future(); }

private:
	void run() override { f(); }

	future_call_with_return_value<R> f;
};

template <typename R>
std::future<R> inline run_call_with_return_value(task_system &system, std::function<R()> fn, const std::shared_ptr<task_set> &set, task_affinity affinity = any_task_worker, task_priority priority = default_task_priority) {
	auto call_task(std::make_shared<future_call_task_with_return_value<R>>(std::move(fn)));
	auto f = call_task->get_future();
	system.push(call_task, set, affinity, priority);
	return f;
}

template <typename R>
std::future<R> inline run_call_with_return_value(task_system &system, std::function<R()> fn, task_affinity affinity = any_task_worker, task_priority priority = default_task_priority) {
	auto call_task(std::make_shared<future_call_task_with_return_value<R>>(std::move(fn)));
	auto f = call_task->get_future();
	system.push(call_task, affinity, priority);
	return f;
}

template <typename R>
std::future<R> inline run_call_with_return_value(std::function<R()> fn, const std::shared_ptr<task_set> &set, task_affinity affinity = any_task_worker, task_priority priority = default_task_priority) { return run_call_with_return_value(g_task_system.get(), std::move(fn), set, affinity, priority); }

template <typename R>
std::future<R> inline run_call_with_return_value(std::function<R()> fn, task_affinity affinity = any_task_worker, task_priority priority = default_task_priority) { return run_call_with_return_value(g_task_system.get(), std::move(fn), affinity, priority); }

//
template <typename R>
std::shared_ptr<future_call_task<R>> bind_task(std::function<R()> f) { return std::make_shared<future_call_task<R>>(f); }
template <typename R>
std::shared_ptr<future_call_task_with_return_value<R>> bind_task_with_return_value(std::function<R()> f) { return std::make_shared<future_call_task_with_return_value<R>>(f); }

} // namespace hg
