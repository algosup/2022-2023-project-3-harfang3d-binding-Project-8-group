// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <map>
#include <string>

namespace hg {

typedef std::map<std::string, std::string> Parameters;

/// Parse a string of parameters.
Parameters ParseParameters(const std::string &parm_string);

} // namespace hg
