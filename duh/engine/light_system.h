// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/scene_system.h"
#include "foundation/frustum.h"
#include "foundation/vector3.h"
#include <memory>
#include <vector>

namespace hg {

class Node;
class Transform;
class Light;

/// Light system
class LightSystem : public std::enable_shared_from_this<LightSystem>, public SceneSystem {
public:
	LightSystem();

	const std::string &GetAspect() const override;

	void AddComponent(std::shared_ptr<Component> component) override;
	void RemoveComponent(const std::shared_ptr<Component> &component) override;

	const std::vector<std::shared_ptr<Node>> &GetLights() const { return lights; }

	/// Get all lights in frustum.
	std::vector<std::shared_ptr<Node>> GetLightsInFrustum(const Vector3 &world_pos, const FrustumPlanes &frustum) const;
	/// Get closest lights.
	std::vector<std::shared_ptr<Node>> GetClosestLights(const Vector3 &world_pos) const;

	void DrawDebugPanel(RenderSystem &render_system) override;
	void DrawDebugVisuals(RenderSystem &render_system) override;

	std::shared_ptr<SceneSystem> shared_from_base() override { return shared_from_this(); }

private:
	std::vector<std::shared_ptr<Node>> lights;

	bool draw_spot_frustum{true};
};

Frustum BuildSpotLightFrustum(const Transform &transform, const Light &light);

} // namespace hg
