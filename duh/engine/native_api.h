// GS Framework - Copyright (C) 2019-2015 Emmanuel Julien. All Rights Reserved.

#pragma once

#include "foundation/filesystem.h"

#if ANDROID
#include "android_native_api.h"
#endif // ANDROID

// published app API
#if HARFANG_PUBLISHED

namespace hg {

// Return a driver to the application external storage.
std::shared_ptr<FileDriver> NewExternalFilesDriver();
// Return a driver able to access the application data files.
std::shared_ptr<FileDriver> NewAppDataDriver();

} // namespace hg

// Entry point for a published native app.
void PublishedMain();

#endif // HARFANG_PUBLISHED
