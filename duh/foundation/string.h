// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace hg {

/*
 * Routine to see if a text string is matched by a wildcard pattern.
 * Returns TRUE if the text is matched, or FALSE if it is not matched
 * or if the pattern is invalid.
 *  *		matches zero or more characters
 *  ?		matches a single character
 *  [abc]	matches 'a', 'b' or 'c'
 *  \c		quotes character c
*/
bool match_wildcard(const char *text, const char *pattern);

enum case_sensitivity {
	insensitive,
	sensitive
};

inline char to_lower(char c) { return c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c; }

inline bool starts_with(const std::string &value, const std::string &prefix, case_sensitivity sensitivity = case_sensitivity::sensitive) {
	if (prefix.size() > value.size())
		return false;

	return sensitivity == case_sensitivity::sensitive
			   ? equal(prefix.begin(), prefix.end(), value.begin(), [](const char &a, const char &b) { return a == b; })
			   : equal(prefix.begin(), prefix.end(), value.begin(), [](const char &a, const char &b) { return to_lower(a) == to_lower(b); });
}

inline bool ends_with(const std::string &value, const std::string &suffix, case_sensitivity sensitivity = case_sensitivity::sensitive) {
	if (suffix.size() > value.size())
		return false;

	return sensitivity == case_sensitivity::sensitive
			   ? equal(suffix.rbegin(), suffix.rend(), value.rbegin(), [](const char &a, const char &b) { return a == b; })
			   : equal(suffix.rbegin(), suffix.rend(), value.rbegin(), [](const char &a, const char &b) { return to_lower(a) == to_lower(b); });
}

/// In-place replace all occurrences of 'what' by 'by'. Returns the number of occurrence replaced.
size_t replace_all(std::string &inplace_replace, const std::string &what, const std::string &by);
inline size_t replace_all(std::string &inplace_replace, const char *what, const char *by) { return replace_all(inplace_replace, std::string(what), std::string(by)); }

template <typename T>
size_t replace_all(std::string &inplace_replace, const T &what, const T &by) {
	size_t count = 0;
	if (what.size() == by.size())
		for (size_t i = 0; i < what.size(); ++i)
			count += replace_all(inplace_replace, what[i], by[i]);
	return count;
}

/// Split std::string against a separator std::string.
std::vector<std::string> split(const std::string &value, const std::string &separator, const std::string &trim = "");

std::string lstrip(const std::string &str, const std::string &pattern = " ");
std::string rstrip(const std::string &str, const std::string &pattern = " ");
std::string strip(const std::string &str, const std::string &pattern = " ");

std::string trim(const std::string &str, const std::string &pattern = " ");
std::string reduce(const std::string &str, const std::string &fill = " ", const std::string &pattern = " ");

/// Join several std::strings with a separator std::string.
template <typename T>
std::string join(T begin_it, T end_it, const std::string &separator) {
	auto count = std::distance(begin_it, end_it);

	if (!count)
		return std::string();
	if (count == 1)
		return *begin_it;

	--end_it;

	std::string out;
	for (auto i = begin_it; i != end_it; ++i)
		out = out + *i + separator;

	return out + *end_it;
}

bool contains(const std::string &in, const std::string &what);

std::string utf16_to_utf8(const std::u16string &str);
std::u16string utf8_to_utf16(const std::string &str);

std::string utf32_to_utf8(const std::u32string &str);
std::u32string utf8_to_utf32(const std::string &str);

#if WIN32
std::string LPWSTR_to_utf8(const wchar_t *str);
#endif

void tolower_inplace(std::string &inplace_str, size_t start = 0, size_t end = 0);
std::string tolower(std::string str, size_t start = 0, size_t end = 0);
void toupper_inplace(std::string &inplace_str, size_t start = 0, size_t end = 0);
std::string toupper(std::string str, size_t start = 0, size_t end = 0);

/// Take a slice from a std::string. 'from < 0' starts 'from' characters from the right of the std::string. 'count < 0' returns all characters after 'from' minus 'count'.
std::string slice(const std::string &str, ptrdiff_t from, ptrdiff_t count = 0);

std::string left(const std::string &str, size_t count);
std::string right(const std::string &str, size_t count);

enum EOLConvention {
	EOLUnix,
	EOLWindows
};

void normalize_eol(std::string &inplace_normalize, EOLConvention = EOLUnix);

std::string strip_prefix(const std::string &str, const std::string &prefix);
std::string strip_suffix(const std::string &str, const std::string &suffix);

#define nullstr std::string()

} // namespace hg
