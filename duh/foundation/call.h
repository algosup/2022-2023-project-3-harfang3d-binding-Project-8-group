// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <functional>
#include <future>

namespace hg {

struct future_call_base {
	virtual ~future_call_base() {}
	virtual void operator()() = 0;
};

// future call, no return value
template <typename R>
struct future_call : future_call_base {
	explicit future_call(std::function<R()> fn_) : fn(std::move(fn_)) {}

	void operator()() override { fn(); }

private:
	std::function<void()> fn;
};

// base class for future call with return value
template <typename R>
struct future_call_with_return_value_base : future_call_base {
	virtual std::future<R> get_future() = 0;
};

// future call with return value
template <typename R>
struct future_call_with_return_value;

template <>
struct future_call_with_return_value<void> : future_call_with_return_value_base<void> {
	explicit future_call_with_return_value(std::function<void()> fn_) : fn(std::move(fn_)) {}

	void operator()() override {
		fn();
		rval.set_value();
	}

	std::future<void> get_future() override { return rval.get_future(); }

private:
	std::function<void()> fn;
	std::promise<void> rval;
};

template <typename R>
struct future_call_with_return_value : future_call_with_return_value_base<R> {
	explicit future_call_with_return_value(std::function<R()> fn_) : fn(std::move(fn_)) {}

	void operator()() override { rval.set_value(fn()); }

	std::future<R> get_future() override { return rval.get_future(); }

private:
	std::function<R()> fn;
	std::promise<R> rval;
};

} // namespace hg
