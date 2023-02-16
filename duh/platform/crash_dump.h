// HARFANG�3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

namespace hg {

/// Install the global platform crash handler.
void InstallCrashHandler();

/// Set the crash report filename.
void SetCrashReportFilename(const char *name);
/// Set the crash dump filename.
void SetCrashDumpFilename(const char *name);

/// Enable crash handler files.
void EnableCrashHandlerFiles(bool enable_crash_report, bool enable_crash_minidump);

} // namespace hg
