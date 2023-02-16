// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <string>
#include <vector>

namespace hg {

struct WatchEvent {
	enum Type {
		FileAdded,
		FileRemoved,
		FileModified
	};

	Type type;
	std::string path;

	bool operator==(const WatchEvent &e) const { return e.type == type && e.path == path; }
};

struct DirectoryWatch {
	virtual ~DirectoryWatch() = default;
	virtual void GetEvents(std::vector<WatchEvent> &out_events) = 0;
	virtual bool IsRunning() const = 0;
};

DirectoryWatch *WatchDirectoryForModification(const std::string &absolute_path_to_directory, bool recursive);

} // namespace hg
