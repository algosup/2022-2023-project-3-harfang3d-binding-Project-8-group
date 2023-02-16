// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "engine/rigid_body_type.h"
#include "foundation/matrix4.h"
#include "foundation/vector3.h"

namespace hg {

/**
	@short Rigid body initial state

	The initial state is defined by all operations taking place on the rigid
	before it actually is added to a physic system.
*/
struct RigidBodyInitialState {
	Vector3 W{0, 0, 0}, V{0, 0, 0}; // angular velocity, linear velocity
	Vector3 F{0, 0, 0}, I{0, 0, 0}, T{0, 0, 0}; // force, impulse, torque

	float sF{0.5f}, dF{0.5f}; // static/dynamic friction
	float lD{0.f}, aD = {0.f}; // linear/angular damping
	float R{0.5f}; // restitution

	RigidBodyType type{RigidBodyDynamic}; // static/kinematic/dynamic

	bool is_sleeping{false}; // is sleeping
	uint8_t axis_lock{0}; // global axis lock
	uint8_t collision_layer{0}; // collision layer
};

/// Rigid body component
class RigidBody : public std::enable_shared_from_this<RigidBody>, public Component {
public:
	RigidBody();

	decl_component_defaults(RigidBody, "RigidBody", "RigidBody");

	comp_decl_member(RigidBody, bool, IsSleeping);

	const Vector3 GetVelocity(const Vector3 &p) const;
	void SetVelocity(const Vector3 &v, const Vector3 &p);

	comp_decl_member(RigidBody, Vector3, LinearVelocity);
	comp_decl_member(RigidBody, Vector3, AngularVelocity);

	comp_decl_member(RigidBody, float, LinearDamping);
	comp_decl_member(RigidBody, float, AngularDamping);

	comp_decl_member(RigidBody, float, StaticFriction);
	comp_decl_member(RigidBody, float, DynamicFriction);

	comp_decl_member(RigidBody, float, Restitution);

	comp_decl_member(RigidBody, RigidBodyType, Type);

	comp_decl_member_get_sync_set_async_noimpl_novar(RigidBody, int, AxisLock);
	comp_decl_member_get_sync_set_async_noimpl_novar(RigidBody, int, CollisionLayer);

	/// Apply a world space impulse (instantaneous change in velocity) on the body center of mass.
	void ApplyLinearImpulse(const Vector3 &I);
	/// Apply a world space force on the body center of mass.
	void ApplyLinearForce(const Vector3 &F);
	/// Apply a world space impulse (instantaneous change in velocity) on the body.
	void ApplyImpulse(const Vector3 &I, const Vector3 &p);
	/// Apply a world space force on the body.
	void ApplyForce(const Vector3 &F, const Vector3 &p);
	/// Apply a world space torque (rotational force) on the body.
	void ApplyTorque(const Vector3 &T);

	/// Reset the body position.
	void ResetWorld(const Matrix4 &M);

	void *impl{nullptr}; // implementation pointer

	/// Return the body initial state, creates it if needed.
	RigidBodyInitialState &GetOrCreateInitialState() const;
	/// Return the body initial state and destroy the local copy.
	RigidBodyInitialState GetAndDeleteInitialState();

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

private:
	mutable std::unique_ptr<RigidBodyInitialState> initial_state;

	uint8_t axis_lock{0};
	uint8_t collision_layer{0};
};

} // namespace hg
