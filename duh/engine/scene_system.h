// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/call_queue_mt.h"
#include "foundation/time.h"

namespace hg {

class task_set;
class Type;

class RenderSystem;

class Component;

/// Implement an aspect of the scene.
class SceneSystem {
public:
	SceneSystem();
	virtual ~SceneSystem() {}

	/// Return the system aspect.
	virtual const std::string &GetAspect() const = 0;

	/// Return the system concrete implementation type.
	inline Type *GetConcreteType() const { return concrete_type; }

	/// System update phase, system and nodes are read-only during this phase.
	virtual void Update(time_ns dt = 0) {}
	/// Wait for the update task set to complete.
	virtual bool WaitUpdate(bool blocking = true);

	/// System commit phase, system and nodes are write-only during this phase.
	virtual void Commit();
	/// Wait for the commit task set to complete.
	virtual bool WaitCommit(bool blocking = true);

	/// Called by the scene at the end of its update to process components about to be added.
	virtual void AddComponent(std::shared_ptr<Component> component);
	/// Called by the scene at the end of its update to process components about to be removed.
	virtual void RemoveComponent(const std::shared_ptr<Component> &component);

	/// Perform system cleanup after a system has been completely emptied of components.
	virtual void Cleanup() {}

	void SetDebugVisuals(bool enabled) { dbg_visual = enabled; }
	bool GetDebugVisuals() const { return dbg_visual; }

	virtual void DrawDebugPanel(RenderSystem &render_system) {}
	virtual void DrawDebugVisuals(RenderSystem &render_system) {}

	virtual std::shared_ptr<SceneSystem> shared_from_base() = 0;

	call_queue_mt commit_queue;

protected:
	Type *concrete_type{nullptr};

	std::shared_ptr<task_set> update, commit;

	bool dbg_visual{false}; // visual debugging

	/// Wait for a task set to complete then reset the set tracking state.
	bool WaitTaskSet(const std::shared_ptr<task_set> &set, bool blocking);
	void SpawnExecuteCommitQueueTask();
};

} // namespace hg

#define decl_system_base_get_set_async_noimpl(SYSTEM, TYPE, NAME)                                                    \
	virtual TYPE Get##NAME() const = 0;                                                                              \
	void Set##NAME(const TYPE &v) { commit_queue.queue<void>(std::bind<void>(&SYSTEM::commit_Set##NAME, this, v)); } \
                                                                                                                     \
protected:                                                                                                           \
	virtual void commit_Set##NAME(const TYPE &v) = 0;

#define decl_system_base_get_set_async(SYSTEM, TYPE, NAME, VAR) \
private:                                                        \
	TYPE VAR;                                                   \
	void commit_Set##NAME(const TYPE &v) { VAR = v; }           \
                                                                \
public:                                                         \
	TYPE Get##NAME() const { return VAR; }                      \
	void Set##NAME(const TYPE &v) { commit_queue.queue<void>(std::bind(&SYSTEM::commit_Set##NAME, this, v)); }

#define decl_system_base_component_get_set_async(SYSTEM, COMP, TYPE, NAME)                                                 \
public:                                                                                                                    \
	virtual TYPE Get##NAME(const COMP *c) const = 0;                                                                       \
	void Set##NAME(COMP *c, const TYPE &v) { commit_queue.queue<void>(std::bind(&SYSTEM::commit_Set##NAME, this, c, v)); } \
                                                                                                                           \
protected:                                                                                                                 \
	virtual void commit_Set##NAME(COMP *c, const TYPE &v) = 0;
