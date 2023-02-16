// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/file_driver.h"
#include "foundation/string.h"

namespace hg {

/**
	@short Rewriter proxy I/O.
	A simple proxy I/O to rewrite filenames before forwarding them to another I/O system.

	The rewriting rule can make use of the following symbols to rewrite a path.
		* %path%: The path to be rewritten.
*/
class RewriterFileDriver : public FileDriver {
public:
	RewriterFileDriver(std::shared_ptr<FileDriver> io, const char *rule) : iofs(io), rewriter_rule(rule) {}

	FileDriverCaps GetCaps() const override { return iofs->GetCaps(); }

	std::shared_ptr<FileHandle> Open(const std::string &path, FileMode mode = FileRead) override { return iofs->Open(RewritePath(path), mode); }
	void Close(FileHandle &handle) override { iofs->Close(handle); }

	bool Delete(const std::string &path) override { return iofs->Delete(RewritePath(path)); }

	size_t Tell(FileHandle &handle) override { return iofs->Tell(handle); }
	size_t Seek(FileHandle &handle, ptrdiff_t offset, SeekRef seek_ref = SeekCurrent) override { return iofs->Seek(handle, offset, seek_ref); }
	size_t Size(FileHandle &handle) override { return iofs->Size(handle); }

	bool IsEOF(FileHandle &handle) override { return iofs->IsEOF(handle); }

	size_t Read(FileHandle &handle, void *p_out, size_t size) override { return iofs->Read(handle, p_out, size); }
	size_t Write(FileHandle &handle, const void *p_out, size_t size) override { return iofs->Write(handle, p_out, size); }

	bool MkDir(const std::string &path) override { return iofs->MkDir(RewritePath(path)); }

private:
	std::shared_ptr<FileDriver> iofs;
	std::string rewriter_rule;

	std::string RewritePath(const std::string &path) const {
		std::string output(rewriter_rule);
		replace_all(output, "%path%", path);
		return output;
	}
};

} // namespace hg
