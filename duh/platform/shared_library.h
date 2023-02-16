// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <string>

namespace hg {

/// Shared library
struct SharedLibrary {
	virtual ~SharedLibrary() {}
	/// Lookup for a function exported by the shared library.
	virtual void *GetFunctionPointer(const std::string &name) = 0;
};

/// Load a shared library.
SharedLibrary *LoadSharedLibrary(const std::string &path);

/// Set the shared library search path.
bool SetSharedLibraryPath(const std::string &path);

} // namespace hg
