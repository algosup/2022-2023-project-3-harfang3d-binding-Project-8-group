// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstdio>
#include <mutex>
#include <string>

namespace hg {

struct LogFile {
	LogFile(const std::string &path_, bool do_timestamp_ = true);
	~LogFile();

	void log(const std::string &msg, bool close = false);

private:
	std::mutex lock;

	FILE *f{nullptr};
	std::string path;

	bool do_timestamp : 1;
	bool initial_write : 1;
};

} // namespace hg
