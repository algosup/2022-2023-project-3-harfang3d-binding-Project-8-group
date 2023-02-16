// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <string>
#include <vector>

namespace hg {

bool IsPathAbsolute(const std::string &path);

std::string BeautifyPath(const std::string &path, bool with_space = true);

std::string FactorizePath(const std::string &path);
std::string CleanPath(const std::string &path);

std::string CutFilePath(const std::string &path);
std::string CutFileName(const std::string &path);
std::string CutFileExtension(const std::string &path);

std::string GetFilePath(const std::string &path);
std::string GetFileName(const std::string &path);
std::string GetFileExtension(const std::string &path);

bool HasFileExtension(const std::string &path);

bool PathStartsWith(const std::string &path, const std::string &with);
std::string PathStripPrefix(const std::string &path, const std::string &prefix);
std::string PathStripSuffix(const std::string &path, const std::string &suffix);
std::string PathJoin(const std::vector<std::string> &elements);

std::string SwapFileExtension(const std::string &path, const std::string &ext);

std::string GetCurrentWorkingDirectory();
bool SetCurrentWorkingDirectory(const std::string &path);

} // namespace hg
