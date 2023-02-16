// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/global_object.h"
#include "foundation/time.h"
#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace hg {

class timer_thread {
public:
	typedef size_t handle;

	static const handle null_handle;

	enum State {
		Stopped,
		Running
	};

	explicit timer_thread(std::string name_ = "Timer") : name(std::move(name_)) {}
	~timer_thread();

	/// Push a periodic call, the first call is done immediately.
	handle run_periodic(const std::function<void()> &call, time_ns period, time_ns initial_delay = 0);
	/// Push a delayed call.
	void run_delayed(const std::function<void()> &call, time_ns delay);

	/// Cancel a periodic call.
	void cancel_periodic(handle);

	/// Start the timer thread, specifying the minimum timer resolution.
	void start(time_ns resolution = time_from_ms(1));
	/// Stop the timer thread.
	void stop();

	bool is_running() const { return state != Stopped; }

	void execute();

private:
	std::string name;
	std::thread thread_;

	std::atomic<State> state{Stopped};
	std::mutex lock;

	time_ns resolution{time_from_ms(1)};

	struct delayed_call {
		delayed_call(const std::function<void()> &c, time_ns d) : call(c), delay(d) {}
		std::function<void()> call;
		time_ns delay;
	};

	struct periodic_call {
		periodic_call(const std::function<void()> &c, time_ns p, time_ns d) : call(c), period(p), delay(d) {}
		std::function<void()> call;
		time_ns period, delay;
	};

	std::vector<delayed_call> delayed_calls;
	std::vector<periodic_call> periodic_calls;
};

extern global_object<timer_thread> g_timer;

} // namespace hg
