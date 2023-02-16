// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/dir_entry.h"
#include "foundation/file_mode.h"
#include "foundation/global_object.h"
#include "foundation/string.h"
#include <memory>
#include <vector>

namespace hg {

class BinaryData;

class FileHandle;
class FileDriver;

/// @cond HIDDEN_SYMBOLS
class MountPoint {
public:
	MountPoint(std::string prefix_, std::shared_ptr<FileDriver> driver_) : prefix(std::move(prefix_)), driver(std::move(driver_)) {}

	inline const std::string &GetPrefix() const { return prefix; }
	inline const std::shared_ptr<FileDriver> &GetDriver() const { return driver; }

	inline std::string StripPrefix(const std::string &path) const { return strip_prefix(path, prefix); }

private:
	std::string prefix;
	std::shared_ptr<FileDriver> driver;
};
/// @endcond

/// Provide access and manages the available file drivers
class Filesystem {
public:
	Filesystem();
	~Filesystem();

	bool Mount(std::shared_ptr<FileDriver> driver);
	bool Mount(std::shared_ptr<FileDriver> driver, const std::string &prefix);
	bool IsPrefixMounted(const std::string &prefix) const { return GetMountPoint(prefix) != nullptr; }

	void Unmount(const std::string &prefix);
	void Unmount(const std::shared_ptr<FileDriver> &driver);
	void UnmountAll();

	std::string MapToAbsolute(const std::string &path) const;
	std::string MapToRelative(const std::string &path) const;
	std::string StripPrefix(const std::string &path) const;
	std::string GetPrefix(const std::string &path) const;

	std::shared_ptr<FileHandle> Open(const std::string &path, FileMode mode = FileRead) const;
	void Close(FileHandle &handle) const;

	std::vector<DirEntry> Dir(const std::string &path, const std::string &wildcard = "*.*", DirEntry::Type filter = DirEntry::All);
	bool MkDir(const std::string &path) const;
	bool IsDir(const std::string &path) const;

	bool Exists(const std::string &path) const;
	bool Delete(const std::string &path) const;

	size_t FileSize(const std::string &path) const;
	bool FileLoad(const std::string &path, BinaryData &data_out) const;
	bool FileLoad(const std::string &path, std::vector<char> &data_out) const;
	bool FileSave(const std::string &path, const BinaryData &data_in) const;
	bool FileSave(const std::string &path, const std::vector<char> &data_in) const;
	bool FileSave(const std::string &path, const void *data_in, size_t size) const;
	bool FileCopy(const std::string &src_path, const std::string &dst_path) const;
	bool FileMove(const std::string &src_path, const std::string &dst_path) const;

	std::string FileToString(const std::string &path) const;
	bool StringToFile(const std::string &path, const std::string &text) const;

	std::shared_ptr<FileDriver> GetPrefixFileDriver(const std::string &prefix) const;

private:
	std::vector<MountPoint> mount_points;
	std::vector<std::shared_ptr<FileDriver>> root_drivers;

	const MountPoint *GetMountPoint(const std::string &path) const;
};

extern global_object<Filesystem> g_fs;

} // namespace hg
