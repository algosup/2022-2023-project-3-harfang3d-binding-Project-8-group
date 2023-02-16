// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/call.h"
#include <memory>
#include <mutex>
#include <vector>

namespace hg {

/// Thread-safe call queue.
class call_queue_mt {
	using queue_type = std::vector<std::unique_ptr<future_call_base>>;

public:
	template <typename R>
	inline std::future<R> queue_with_return_value(std::function<R()> f) {
		std::lock_guard<std::mutex> lock(access);
		auto call = new future_call_with_return_value<R>(std::move(f));
		q.emplace_back(call);
		return call->get_future();
	}

	template <typename R>
	inline void queue(std::function<R()> f) {
		std::lock_guard<std::mutex> lock(access);
		q.emplace_back(new future_call<R>(std::move(f)));
	}

	inline queue_type::size_type size() {
		std::lock_guard<std::mutex> lock(access);
		return q.size();
	}

	void execute() {
		queue_type exec_q;

		while (true) {
			{
				std::lock_guard<std::mutex> lock(access);
				exec_q = std::move(q);
				q.clear();
			}

			if (!exec_q.size())
				break;

			for (auto &c : exec_q)
				(*c)();

			exec_q.clear();
		}
	}

private:
	std::mutex access;
	queue_type q;
};

} // namespace hg
