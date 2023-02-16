// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/scene_system.h"

namespace hg {

/// System tracking camera components in a Scene
class CameraSystem : public std::enable_shared_from_this<CameraSystem>, public SceneSystem {
public:
	CameraSystem();

	const std::string &GetAspect() const override;

	void AddComponent(std::shared_ptr<Component> c) override;
	void RemoveComponent(const std::shared_ptr<Component> &c) override;

	std::shared_ptr<SceneSystem> shared_from_base() override { return shared_from_this(); }
};

} // namespace hg
