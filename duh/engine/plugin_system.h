// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/global_object.h"
#include "foundation/math.h"
#include "platform/plugin.h"
#include <memory>
#include <string>
#include <vector>

namespace hg {

static const int plugin_host_version = 1;

struct PluginSystem {
	std::vector<std::unique_ptr<Plugin>> plugins;
	std::vector<std::string> default_search_paths;
	std::vector<std::string> blacklist;
};

extern global_object<PluginSystem> g_plugin_system;

/// Load engine plugins.
uint32_t LoadPlugins();
uint32_t LoadPlugins(std::string path);
/// Unload engine plugins.
void UnloadPlugins();

/// Add entries to the plugins blacklist.
void AddToPluginsBlacklist(const std::vector<std::string> &blacklist);

/// To be called from the plugin at load time.
void PluginSetupHost(void *host);

} // namespace hg
