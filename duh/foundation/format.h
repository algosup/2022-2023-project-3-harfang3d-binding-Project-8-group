// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/string.h"
#include <sstream>

namespace std {

string to_string(const void *p);

inline string to_string(const string &v) { return v; }
inline string to_string(const char *v) { return v; }

} // namespace std

namespace hg {

/// String formatting class
struct format {
	explicit format(std::string _text) : text(std::move(_text)) {}
	explicit format(const char *_text) : text(_text) {}

	operator const std::string &() const { return text; }

	const char *c_str() const { return text.c_str(); }
	const std::string &str() const { return text; }

	template <typename T>
	format &arg(const T &v) {
		return replace_next_token(std::to_string(v));
	}

	format &arg(int v, int width, char fill = '0') {
		std::stringstream ss;
		ss.width(width);
		ss.fill(fill);
		ss << v;
		return replace_next_token(ss.str());
	}

	format &arg(float v, int precision) {
		std::stringstream ss;
		ss.precision(precision);
		ss << v;
		return replace_next_token(ss.str());
	}

private:
	format &replace_next_token(const std::string &by) {
		std::string token = "%";
		token += std::to_string(i++);
		replace_all(text, token, by);
		return *this;
	}

	std::string text;
	int i{1};
};

} // namespace hg
