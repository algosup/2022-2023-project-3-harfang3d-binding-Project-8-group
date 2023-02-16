// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/call_queue_mt.h"

namespace hg {

/// Initialize main thread related informations.
void init_main_thread();

/// Return true if the calling thread is the main thread.
bool is_main_thread();

/**
	@short Return the main thread call queue.

	This queue is periodically consumed by the main thread from future::wait().

	This is required for OSX to be able to perform UI calls from the main thread
	while still preserving a sane multi-threaded API.
*/
extern call_queue_mt *main_thread_call_queue;

/// Queue a call to be executed on the main thread.
void queue_on_main_thread(future_call<void> *call);

/// Queue a call to be executed on the main thread.
template <typename R>
std::future<R> queue_on_main_thread(future_call<R> *call) {
	auto r = call->get_future();

	if (is_main_thread())
		call->execute();
	else
		main_thread_call_queue->queue(call);

	return r;
}

/// Queue a call to be executed on the main thread.
void queue_on_main_thread(std::function<void()> f);

/// Queue a call to be executed on the main thread.
template <typename R>
std::future<R> queue_on_main_thread(std::function<R()> f) {
	return queue_on_main_thread<R>(new future_call<R>(f));
}

/// Yield execution if caller is the main thread.
void yield_if_main_thread();

} // namespace hg
