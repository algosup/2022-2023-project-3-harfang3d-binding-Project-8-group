// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/scene_system.h"
#include "engine/transform.h"
#include "engine/transform_modifier.h"
#include "foundation/task.h"
#include <unordered_map>

namespace hg {

class Node;

/// Transform system
class TransformSystem : public std::enable_shared_from_this<TransformSystem>, public SceneSystem {
public:
	friend class Transform;

	TransformSystem();

	const std::string &GetAspect() const override;

	void AddComponent(std::shared_ptr<Component> component) override;
	void RemoveComponent(const std::shared_ptr<Component> &component) override;

	void ComputeTransform(Transform &transform);
	void CommitTransform(Transform &transform);

	void Commit() override;

	void ResetWorldMatrix(const std::shared_ptr<Transform> &transform, const Matrix4 &m);

	void DrawDebugPanel(RenderSystem &render_system) override;

	std::shared_ptr<SceneSystem> shared_from_base() override { return shared_from_this(); }

private:
	std::unordered_map<std::shared_ptr<Transform>, Matrix4> reset_world_matrix;
	uint32_t reset_world_matrix_count{0};

	void DoCommit();

	void StartTransformTasks();
	std::shared_ptr<task> root_transform_barrier;
	std::vector<std::shared_ptr<Node>> transformed_nodes;

	void ApplyModifiers();
	std::vector<std::shared_ptr<TransformModifier>> transform_modifiers;

	std::vector<Matrix4> previous, current, next;
};

} // namespace hg
