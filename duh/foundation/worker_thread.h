// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/call.h"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <vector>

namespace hg {

class worker_thread {
	using calls_queue = std::vector<std::unique_ptr<future_call_base>>;

public:
	enum State {
		Stopped,
		Starting,
		Running,
		Stopping
	};

	worker_thread(bool auto_start = false);
	~worker_thread() { stop(); }

	void execute();

	template <typename R>
	void run(std::function<R()> fn) {
		std::lock_guard<std::mutex> guard(calls_mutex);
		calls.emplace_back(new future_call<R>(std::move(fn)));
		calls_cv.notify_one();
	}

	template <typename R>
	std::future<R> run_with_return_value(std::function<R()> fn) {
		std::lock_guard<std::mutex> guard(calls_mutex);

		auto call = new future_call_with_return_value<R>(std::move(fn));
		calls.emplace_back(call);

		auto r = call->get_future();
		calls_cv.notify_one();

		return r;
	}

	virtual void start();
	virtual void stop();

	bool is_running() const { return state >= Running; }
	bool is_started() const { return state != Stopped; }

	virtual void on_start_execution() {}
	virtual void on_end_execution() {}

private:
	std::atomic<State> state;
	std::thread thread;

	std::mutex calls_mutex;
	std::condition_variable calls_cv;

	calls_queue calls;
};

} // namespace hg
