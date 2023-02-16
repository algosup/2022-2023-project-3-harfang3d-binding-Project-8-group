// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "engine/group.h"
#include "engine/scene_system.h"
#include <string>

namespace hg {

class Scene;
class RenderSystem;

/// Instance system
class InstanceSystem : public std::enable_shared_from_this<InstanceSystem>, public SceneSystem {
public:
	InstanceSystem(std::shared_ptr<Scene> scene, std::shared_ptr<RenderSystem> render_system);

	const std::string &GetAspect() const override;

	void AddComponent(std::shared_ptr<Component> c) override;
	void RemoveComponent(const std::shared_ptr<Component> &c) override;

	std::shared_ptr<SceneSystem> shared_from_base() override { return shared_from_this(); }

	void SetInstanceDepthLimit(size_t limit) { instance_depth_limit = limit; }
	size_t GetInstanceDepthLimit() const { return instance_depth_limit; }

private:
	size_t instance_depth_limit{4};

	std::weak_ptr<Scene> scene;
	std::shared_ptr<RenderSystem> render_system;

	bool Instantiate(Component &c);
	void Deinstantiate(Component &c);
};

} // namespace hg
