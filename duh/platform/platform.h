// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <string>

namespace hg {

/// Return the platform locale.
std::string GetPlatformLocale();

/// Open a file dialog
bool OpenFolderDialog(const std::string &title, std::string &output, const std::string &initial_dir = {});
/// Open a file dialog
bool OpenFileDialog(const std::string &title, const std::string &filter, std::string &output, const std::string &initial_dir = {});
/// Open a save file dialog
bool SaveFileDialog(const std::string &title, const std::string &filter, std::string &output, const std::string &initial_dir = {});

/// Platform specific initialization.
bool InitPlatform();

/// Drop to the debugger, throws an exception otherwise.
void DebugBreak();

/// Helper function for platform specific code.
#if WIN32
std::string GetLastError_Win32();
#endif

} // namespace hg
