// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/transform_modifier.h"

namespace hg {

class Target : public std::enable_shared_from_this<Target>, public TransformModifier {
public:
	decl_component_defaults(Target, "Target", "TransformModifier");

	Target();
	~Target();

	std::shared_ptr<Node> GetTarget() const { return target.lock(); }
	void SetTarget(std::shared_ptr<Node> target);

	void ModifyTransform(Matrix4 &world) override;

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

private:
	std::weak_ptr<Node> target;
	void commit_SetTarget(std::shared_ptr<Node> target);
};

} // namespace hg
