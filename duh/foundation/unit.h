// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>

namespace hg {

template <typename T>
constexpr T Deg(T v) {
	static_assert(std::is_floating_point<T>::value, "Expected floating point type");
	return v / T(180) * T(3.1415926535);
}
template <typename T>
constexpr T Rad(T v) {
	static_assert(std::is_floating_point<T>::value, "Expected floating point type");
	return v;
}

template <typename T>
constexpr T DegreeToRadian(T v) {
	static_assert(std::is_floating_point<T>::value, "Expected floating point type");
	return v / T(180) * T(3.1415926535);
}
template <typename T>
constexpr T RadianToDegree(T v) {
	static_assert(std::is_floating_point<T>::value, "Expected floating point type");
	return v / T(3.1415926535) * T(180);
}

template <typename T>
constexpr T Sec(T v) { return v; }

template <typename T>
constexpr T Csec(T v) {
	static_assert(std::is_floating_point<T>::value, "Expected floating point type");
	return v * T(0.01);
}
template <typename T>
constexpr T Ms(T v) {
	static_assert(std::is_floating_point<T>::value, "Expected floating point type");
	return v * T(0.001);
}

template <typename T>
constexpr T Ton(T v) { return v * T(1000); }
template <typename T>
constexpr T Kg(T v) { return v; }

template <typename T>
constexpr T G(T v) {
	static_assert(std::is_floating_point<T>::value, "Expected floating point type");
	return v * T(0.001);
}

template <typename T>
constexpr T Km(T v) { return v * T(1000); }
template <typename T>
constexpr T Mtr(T v) { return v; }

template <typename T>
constexpr T Cm(T v) {
	static_assert(std::is_floating_point<T>::value, "Expected floating point type");
	return v * T(0.01);
}
template <typename T>
constexpr T Mm(T v) {
	static_assert(std::is_floating_point<T>::value, "Expected floating point type");
	return v * T(0.001);
}
template <typename T>
constexpr T Inch(T v) {
	static_assert(std::is_floating_point<T>::value, "Expected floating point type");
	return v * T(0.0254);
}

inline constexpr size_t KB(const size_t size) { return size * 1024; }
inline constexpr size_t MB(const size_t size) { return size * 1024 * 1024; }

template <typename T>
std::string FormatMemorySize(T v_) {
	std::ostringstream str;
	auto v = float(v_);

	if (v < 0) {
		str << "-";
		v = -v;
	}

	if (v < T(1024))
		str << std::fixed << std::setprecision(0) << v << "B";
	else if (v < T(1024 * 1024))
		str << std::fixed << std::setfill('0') << std::setprecision(1) << float(v) / float(1024) << "KB";
	else
		str << std::fixed << std::setfill('0') << std::setprecision(1) << float(v) / float(1024 * 1024) << "MB";

	return str.str();
}

template <typename T>
std::string FormatCount(T v_) {
	std::ostringstream str;
	auto v = float(v_);

	if (v < 0) {
		str << "-";
		v = -v;
	}

	if (v < T(1000))
		str << v;
	if (v < T(1000000))
		str << std::fixed << std::setfill('0') << std::setprecision(1) << float(v) / 1000.f << "K";
	else
		str << std::fixed << std::setfill('0') << std::setprecision(1) << float(v) / 1000000.f << "M";

	return str.str();
}

} // namespace hg
