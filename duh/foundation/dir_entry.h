// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <string>

namespace hg {

struct DirEntry {
	enum Type {
		Invalid,
		File,
		Directory,
		All
	};

	Type type{Invalid};
	std::string name;
};

} // hg
