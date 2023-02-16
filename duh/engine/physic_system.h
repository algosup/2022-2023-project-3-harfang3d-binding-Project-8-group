// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/collision.h"
#include "engine/collision_pair.h"
#include "engine/rigid_body_type.h"
#include "engine/scene_system.h"
#include "foundation/signal.h"
#include <vector>

namespace hg {

class Node;
class Joint;
class RigidBody;

struct PhysicTrace {
	Vector3 p, n;
	Node *i{nullptr};
};

/// Physic system interface.
class PhysicSystem : public SceneSystem {
public:
	PhysicSystem();

	const std::string &GetAspect() const override;

	/// Add a component to the system.
	void AddComponent(std::shared_ptr<Component> component) override;
	/// Remove a component from the system.
	void RemoveComponent(const std::shared_ptr<Component> &component) override;

	/// Return the implementation name.
	virtual const std::string &GetImplementationName() const = 0;

	decl_system_base_get_set_async(PhysicSystem, float, Timestep, timestep);
	decl_system_base_get_set_async(PhysicSystem, bool, ForceRigidBodyToSleepOnCreation, force_sleep_body);
	decl_system_base_get_set_async(PhysicSystem, uint32_t, ForceRigidBodyAxisLockOnCreation, force_axis_lock);

	decl_system_base_get_set_async_noimpl(PhysicSystem, Vector3, Gravity);

	/// Raycast world.
public:
	virtual bool Raycast(const Vector3 &start, const Vector3 &direction, PhysicTrace &hit, uint8_t collision_layer_mask = 0xff, float max_distance = -1) = 0;

	///
	decl_system_base_component_get_set_async(PhysicSystem, RigidBody, bool, RigidBodyIsSleeping);

	decl_system_base_component_get_set_async(PhysicSystem, RigidBody, Vector3, RigidBodyAngularVelocity);
	decl_system_base_component_get_set_async(PhysicSystem, RigidBody, Vector3, RigidBodyLinearVelocity);

	decl_system_base_component_get_set_async(PhysicSystem, RigidBody, float, RigidBodyLinearDamping);
	decl_system_base_component_get_set_async(PhysicSystem, RigidBody, float, RigidBodyAngularDamping);

	decl_system_base_component_get_set_async(PhysicSystem, RigidBody, float, RigidBodyStaticFriction);
	decl_system_base_component_get_set_async(PhysicSystem, RigidBody, float, RigidBodyDynamicFriction);

	decl_system_base_component_get_set_async(PhysicSystem, RigidBody, float, RigidBodyRestitution);

	decl_system_base_component_get_set_async(PhysicSystem, RigidBody, RigidBodyType, RigidBodyType);

	decl_system_base_component_get_set_async(PhysicSystem, RigidBody, uint8_t, RigidBodyAxisLock);
	decl_system_base_component_get_set_async(PhysicSystem, RigidBody, uint8_t, RigidBodyCollisionLayer);

public:
	virtual Vector3 GetRigidBodyVelocity(const RigidBody *body, const Vector3 &p) const = 0;
	void SetRigidBodyVelocity(RigidBody *body, const Vector3 &V, const Vector3 &p) { commit_queue.queue<void>(std::bind(&PhysicSystem::commit_SetRigidBodyVelocity, this, body, V, p)); }

protected:
	virtual void commit_SetRigidBodyVelocity(RigidBody *body, const Vector3 &V, const Vector3 &p) = 0;

public:
	void RigidBodyApplyLinearImpulse(RigidBody *body, const Vector3 &I);
	void RigidBodyApplyLinearForce(RigidBody *body, const Vector3 &F);
	void RigidBodyApplyTorque(RigidBody *body, const Vector3 &T);

	void RigidBodyApplyImpulse(RigidBody *body, const Vector3 &I, const Vector3 &p);
	void RigidBodyApplyForce(RigidBody *body, const Vector3 &F, const Vector3 &p);

	void RigidBodyResetWorld(RigidBody *body, const Matrix4 &M);

protected:
	virtual void commit_RigidBodyApplyLinearImpulse(RigidBody *body, const Vector3 &I) = 0;
	virtual void commit_RigidBodyApplyLinearForce(RigidBody *body, const Vector3 &F) = 0;
	virtual void commit_RigidBodyApplyTorque(RigidBody *body, const Vector3 &T) = 0;

	virtual void commit_RigidBodyApplyImpulse(RigidBody *body, const Vector3 &I, const Vector3 &p) = 0;
	virtual void commit_RigidBodyApplyForce(RigidBody *body, const Vector3 &F, const Vector3 &p) = 0;

	virtual void commit_RigidBodyResetWorld(RigidBody *body, const Matrix4 &M) = 0;

public:
	void OnCollisionModified(const std::shared_ptr<Collision> &collision);
	void OnJointModified(const std::shared_ptr<Joint> &joint);

	/// Return collision pairs for the last update.
	const std::vector<CollisionPair> &GetCollisionPairs() const { return collision_pairs[current_collision_pair_array_index]; }
	/// Return collision pairs involving a specific node for the last update.
	std::vector<CollisionPair> GetCollisionPairs(const std::shared_ptr<Node> &node) const;

	/// Return true if the system has at least one collision pair involving a specified node.
	bool HasCollided(const std::shared_ptr<Node> &node) const;
	bool HasCollided(const std::shared_ptr<Node> &node_a, const std::shared_ptr<Node> &node_b) const;

	void SetCollisionLayerPairState(uint16_t layer_a, uint16_t layer_b, bool enable_collision);
	bool GetCollisionLayerPairState(uint16_t layer_a, uint16_t layer_b) const;

protected:
	std::vector<std::shared_ptr<RigidBody>> bodies_to_create, bodies_to_delete; // shared is required to account for the one frame lag until the list is processed
	std::vector<std::shared_ptr<RigidBody>> bodies_requiring_collision_update;

	std::vector<std::shared_ptr<Collision>> collision_to_create, collision_to_delete;

	std::vector<std::shared_ptr<Joint>> joints_to_create, joint_to_delete, joints_requiring_update;

	std::vector<CollisionPair> collision_pairs[2];
	uint8_t current_collision_pair_array_index;

	uint8_t layer_to_layer_collision_bitmask[8]; // with which other layer will each layer collide

	virtual void OnCollisionLayerMaskModified() {}
};

class ComponentFactory;

/// Register physic system components into the engine factory.
void RegisterPhysicSystemComponents(ComponentFactory &factory);

} // namespace hg
