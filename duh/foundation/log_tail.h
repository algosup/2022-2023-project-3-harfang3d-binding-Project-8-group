// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <string>

namespace hg {

struct LogTail {
public:
	static const int size = 8192;

	/// Write to tail content.
	void Write(const std::string &msg);
	/// Get static tail content.
	inline const char *c_str() const { return tail; }

private:
	char tail[size + 1]{0};
	size_t cursor{0};
};

} // namespace hg
