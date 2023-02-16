// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <string>

namespace hg {

struct LogScope {
	LogScope(const std::string &enter, const std::string &exit);
	~LogScope();

private:
	std::string exit;
};

} // namespace hg
