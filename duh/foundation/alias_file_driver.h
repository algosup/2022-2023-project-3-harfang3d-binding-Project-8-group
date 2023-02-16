// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/file_driver.h"
#include <map>
#include <string>

namespace hg {

/**
	@short Alias proxy I/O.
	A simple proxy I/O to provide filename aliasing to another I/O system.
*/
class AliasFileDriver : public FileDriver {
public:
	AliasFileDriver(FileDriver *io) : iofs(io) {}

	std::shared_ptr<FileHandle> Open(const std::string &path, FileMode mode = FileRead) override { return iofs->Open(ResolveAlias(path), mode); }
	void Close(FileHandle &handle) override { iofs->Close(handle); }

	bool Delete(const std::string &path) override { return iofs->Delete(ResolveAlias(path)); }

	size_t Tell(FileHandle &handle) override { return iofs->Tell(handle); }
	size_t Seek(FileHandle &handle, ptrdiff_t offset, SeekRef seek_ref = SeekCurrent) override { return iofs->Seek(handle, offset, seek_ref); }

	size_t Read(FileHandle &handle, void *p_out, size_t size) override { return iofs->Read(handle, p_out, size); }
	size_t Write(FileHandle &handle, const void *p_out, size_t size) override { return iofs->Write(handle, p_out, size); }

	bool RegisterAlias(const std::string &alias, const std::string &source) {
		if (alias_map.find(alias) == alias_map.end())
			return false;
		alias_map[alias] = source;
		return true;
	}

private:
	std::shared_ptr<FileDriver> iofs;
	std::map<std::string, std::string> alias_map;

	std::string ResolveAlias(const std::string &path) const {
		auto alias = alias_map.find(path);
		if (alias == alias_map.end())
			return path;
		return (*alias).second;
	}
};

} // namespace hg
