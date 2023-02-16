// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstdint>
#include <limits>

namespace hg {

typedef int64_t time_ns;

extern time_ns time_undefined;

constexpr float time_to_sec_f(time_ns t) { return float(t) / 1000000000.f; }
constexpr float time_to_ms_f(time_ns t) { return float(t) / 1000000.f; }
constexpr float time_to_us_f(time_ns t) { return float(t) / 1000.f; }

constexpr int64_t time_to_day(time_ns t) { return t / (1000000000LL * 60LL * 60LL * 24LL); }
constexpr int64_t time_to_hour(time_ns t) { return t / (1000000000LL * 60LL * 60LL); }
constexpr int64_t time_to_min(time_ns t) { return t / (1000000000LL * 60LL); }
constexpr int64_t time_to_sec(time_ns t) { return t / 1000000000LL; }
constexpr int64_t time_to_ms(time_ns t) { return t / 1000000LL; }
constexpr int64_t time_to_us(time_ns t) { return t / 1000LL; }
constexpr int64_t time_to_ns(time_ns t) { return t; }

constexpr time_ns time_from_sec_f(float sec) { return time_ns(sec * 1000000000.f); }
constexpr time_ns time_from_ms_f(float ms) { return time_ns(ms * 1000000.f); }
constexpr time_ns time_from_us_f(float us) { return time_ns(us * 1000.f); }

constexpr time_ns time_from_day(int64_t day) { return day * (1000000000LL * 60LL * 60LL * 24LL); }
constexpr time_ns time_from_hour(int64_t hour) { return hour * (1000000000LL * 60LL * 60LL); }
constexpr time_ns time_from_min(int64_t min) { return min * (1000000000LL * 60LL); }
constexpr time_ns time_from_sec(int64_t sec) { return sec * 1000000000LL; }
constexpr time_ns time_from_ms(int64_t ms) { return ms * 1000000LL; }
constexpr time_ns time_from_us(int64_t us) { return us * 1000LL; }
constexpr time_ns time_from_ns(int64_t ns) { return ns; }

time_ns time_now();

} // namespace hg
