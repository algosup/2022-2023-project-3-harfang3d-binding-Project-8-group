// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

namespace hg {

enum FileMode {
	FileRead,
	FileWrite
};

enum FileDriverCaps {
	FileDriverIsCaseSensitive = 0x01,
	FileDriverCanRead = 0x02,
	FileDriverCanWrite = 0x04,
	FileDriverCanSeek = 0x08,
	FileDriverCanDelete = 0x10,
	FileDriverCanMkDir = 0x20
};

} // namespace hg
