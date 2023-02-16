// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace hg {

class Component;

enum ComponentCompatibility {
	ComponentNonCompatible = 0x00,
	ComponentSceneCompatible = 0x01,
	ComponentNodeCompatible = 0x02
};

/// Describe a component's name, aspect and compatibility
struct ComponentInfo {
	std::string name, aspect;
	std::function<std::shared_ptr<Component>()> make;

	ComponentCompatibility compatibility{ComponentNonCompatible};
	bool allows_multiple_instance{false};

	std::vector<std::string> dependencies;
};

/// Provides a mean to create a concrete component from its name
class ComponentFactory {
public:
	/// Register a new component in the factory.
	std::shared_ptr<ComponentInfo> Register(const std::string &name, const std::string &aspect, std::function<std::shared_ptr<Component>()> make, ComponentCompatibility compatibility, bool allows_multiple_instance = true, const std::vector<std::string> &dependencies = {});
	/// Unregister a component from the factory.
	void Unregister(const std::string &name);

	/// Return the available component info structures.
	inline const std::vector<std::shared_ptr<ComponentInfo>> &GetComponentInfos() const { return component_infos; }

	/// Make a new instance of the component.
	std::shared_ptr<Component> Make(const std::string &name) const;
	/// Return information on the component.
	std::shared_ptr<ComponentInfo> GetComponentInfo(const std::string &name) const;

private:
	std::vector<std::shared_ptr<ComponentInfo>> component_infos;
};

extern ComponentFactory *g_component_factory;

void RegisterCoreComponents(ComponentFactory &factory);

} // namespace hg
