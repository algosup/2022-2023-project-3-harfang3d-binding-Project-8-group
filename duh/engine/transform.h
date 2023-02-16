// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "foundation/matrix3.h"
#include "foundation/matrix4.h"
#include "foundation/vector3_api.h"
#include <memory>
#include <string>

namespace hg {

/// Transformation
class Transform : public std::enable_shared_from_this<Transform>, public Component {
public:
	friend class TransformSystem;
	friend struct TransformBatchTask;

	decl_component_defaults(Transform, "Transform", "Transform");

	Transform();
	Transform(const Vector3 &pos, const Vector3 &rot = Vector3::Zero, const Vector3 &scl = Vector3::One);
	explicit Transform(const Matrix4 &world);

	std::shared_ptr<Node> GetParent() const { return parent.lock(); }
	void SetParent(std::shared_ptr<Node> parent);

	/// Get previous world matrix.
	Matrix4 GetPreviousWorld() const;
	/// Get world matrix.
	Matrix4 GetWorld() const;

	comp_decl_member_get_sync_set_async(Transform, Vector3, Position, position);
	comp_decl_member_get_sync_set_async(Transform, Vector3, Rotation, rotation);
	comp_decl_member_get_sync_set_async(Transform, Vector3, Scale, scale);

	void SetRotationMatrix(const Matrix3 &m);

	/// Set the transform local matrix.
	void SetLocal(const Matrix4 &m);
	/// Set the transform world matrix.
	void SetWorld(const Matrix4 &m);
	/// Offset transformation position in world space.
	void OffsetWorld(const Matrix4 &m);

	/// Transform a local point to world.
	Vector3 TransformLocalPoint(const Vector3 &local_point) const { return local_point * GetWorld(); }

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

private:
	uint32_t id; // index into the transform system

	std::weak_ptr<Node> parent;
	void commit_SetParent(std::shared_ptr<Node> parent);

	void commit_SetLocal(const Matrix4 &m);
	void commit_SetWorld(const Matrix4 &m);
};

} // namespace hg
