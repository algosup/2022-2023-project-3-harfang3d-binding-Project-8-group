// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/string.h"
#include "foundation/math.h"
#include "platform/shared_library.h"
#include <memory>

namespace hg {

static const int PluginBuildDebug = 0x1;
static const int PluginBuildRelease = 0x2;
static const int PluginBuildMixedRelease = 0x4;

static const int PluginAPIVersion = 1;

//
extern "C" {

/// Plugin interface (version 1)
struct PluginDescription {
	unsigned int plugin_api_vers; // must stay the first field in structure

	const char *name;
	const char *vers;
	const char *desc;

	unsigned int host_vers;

	const char *compiler;
	unsigned int build;
};

} // extern "C"

//
enum PluginLoadCode {
	PluginLoadOk,
	PluginLoadUnsupportedEnvironmentVersion
};

/// Return the plugin interface.
typedef const PluginDescription fn_plugin_get_description();
/// Load a plugin (environment v1).
typedef PluginLoadCode fn_plugin_load(void *host);
/// Unload a plugin.
typedef void fn_plugin_unload();

//
#if WIN32
#define GS_DLL_EXPORT __declspec(dllexport)
#else /* POSIX */
#define GS_DLL_EXPORT __attribute__((visibility("default")))
#endif

// Compiler name
#if defined(__clang__)
#define COMPILER_NAME "Clang"
#elif defined(__ICC) || defined(__INTEL_COMPILER)
#define COMPILER_NAME "ICC"
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER_NAME "GCC"
#elif defined(__HP_cc) || defined(__HP_aCC)
#define COMPILER_NAME "aCC"
#elif defined(__IBMC__) || defined(__IBMCPP__)
#define COMPILER_NAME "IBMCPP"
#elif defined(_MSC_VER)
#define COMPILER_NAME "MSVC"
#elif defined(__PGI)
#define COMPILER_NAME "PGCPP"
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#define COMPILER_NAME "SUNPRO_CC"
#else
#define COMPILER_NAME "unknown"
#endif

#if defined(MIXED_RELEASE)
#define GS_PLUGIN_BUILD_FLAGS PluginBuildMixedRelease
#elif defined(NDEBUG)
#define GS_PLUGIN_BUILD_FLAGS PluginBuildRelease
#else
#define GS_PLUGIN_BUILD_FLAGS PluginBuildDebug
#endif

#define GS_DECLARE_PLUGIN_V1(PLUGIN_NAME, PLUGIN_VERS, PLUGIN_DESC, HOST_VERSION) \
                                                                                  \
	static void OnPluginLoad(void *host);                                         \
	static void OnPluginUnload();                                                 \
                                                                                  \
	extern "C" {                                                                  \
                                                                                  \
	GS_DLL_EXPORT PluginDescription PluginGetDescription() {                      \
		PluginDescription desc;                                                   \
		desc.plugin_api_vers = PluginAPIVersion;                                  \
                                                                                  \
		desc.name = PLUGIN_NAME;                                                  \
		desc.vers = PLUGIN_VERS;                                                  \
		desc.desc = PLUGIN_DESC;                                                  \
                                                                                  \
		desc.host_vers = HOST_VERSION;                                            \
                                                                                  \
		desc.compiler = COMPILER_NAME;                                            \
		desc.build = GS_PLUGIN_BUILD_FLAGS;                                       \
                                                                                  \
		return desc;                                                              \
	}                                                                             \
                                                                                  \
	GS_DLL_EXPORT PluginLoadCode PluginLoad(void *host) {                         \
		OnPluginLoad(host);                                                       \
		return PluginLoadOk;                                                      \
	}                                                                             \
                                                                                  \
	GS_DLL_EXPORT void PluginUnload() { OnPluginUnload(); }                       \
                                                                                  \
	} // extern "C"\

//
struct Plugin {
	std::string path;
	std::unique_ptr<SharedLibrary> shared_lib;

	PluginDescription desc;

	fn_plugin_load *load;
	fn_plugin_unload *unload;
};

} // namespace hg
