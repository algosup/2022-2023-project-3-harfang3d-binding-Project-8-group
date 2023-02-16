// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "engine/animation.h"
#include "engine/scene_system.h"
#include "foundation/minmax.h"
#include "foundation/reflection.h"
#include "foundation/signal.h"

namespace hg {

class Node;
class Group;

class VM;
struct ScriptLogicDlgts;

struct SceneSerializationState;
struct SceneDeserializationState;

struct SceneBoundAnim {
private:
	friend class Scene;

	time_ns t;
	int32_t t_scale;

	time_ns start, end;
	AnimLoop loop;

	std::vector<std::shared_ptr<BoundAnim>> bound_anims;
};

// Models a 3D scene and manages its Node actors
class Scene : public std::enable_shared_from_this<Scene> {
public:
	enum Phase {
		PhaseNone,
		PhaseUpdate,
		PhaseWaitUpdate,
		PhaseCommit,
		PhaseWaitCommit
	};

	Scene();
	virtual ~Scene();

	/// Return the current camera node.
	const std::shared_ptr<Node> &GetCurrentCamera() const { return current_camera; }
	/// Set a node as the current camera.
	void SetCurrentCamera(std::shared_ptr<Node> node);

	/// Add a group to the scene.
	void AddGroup(std::shared_ptr<Group> group);
	/// Remove a group from the scene.
	void RemoveGroup(const std::shared_ptr<Group> &group);
	/// Find a group in the scene.
	std::shared_ptr<Group> FindGroup(const std::string &name) const;

	/// Return a list of all groups containing a reference to a specific node.
	std::vector<std::shared_ptr<Group>> GetNodeGroupList(const std::shared_ptr<Node> &node) const;
	/// Remove a node from all scene groups.
	void UnregisterNodeFromGroups(const std::shared_ptr<Node> &node) const;

	/// Return a list of all groups in the scene.
	const std::vector<std::shared_ptr<Group>> &GetGroups() const { return groups; }

	/// Activate all group members.
	void GroupMembersSetActive(const Group &group, bool active = true);
	/// Remove a group and its members from the scene.
	void RemoveGroupAndMembers(const std::shared_ptr<Group> &group);

	/// Clear the scene from all its components, groups and nodes.
	void Clear();
	/// Clear, commit and wait for commit.
	void Dispose();
	/// Return true if all scene components and nodes are ready.
	bool IsReady() const;

	/// Add a system to the scene.
	void AddSystem(std::shared_ptr<SceneSystem> system);
	/// Return the first scene system with the specified name.
	std::shared_ptr<SceneSystem> GetSystem(const std::string &name) const;

	template <class T>
	std::shared_ptr<T> GetSystem() const {
		auto i = std::find_if(systems.begin(), systems.end(), [](const std::shared_ptr<SceneSystem> &s) {
			static auto type = g_type_registry.get().GetType<T>();
			return s->GetConcreteType() == type;
		});
		return i != systems.end() ? std::static_pointer_cast<T>(*i) : nullptr;
	}

	/// Return all scene systems.
	const std::vector<std::shared_ptr<SceneSystem>> &GetSystems() const { return systems; }

	/// Add a node to the scene.
	void AddNode(std::shared_ptr<Node> node);
	/// Remove a node from the scene.
	void RemoveNode(const std::shared_ptr<Node> &node);

	/// Add a component to the scene systems.
	void AddComponentToSystems(std::shared_ptr<Component> component);
	/// Remove a component from the scene systems.
	void RemoveComponentFromSystems(const std::shared_ptr<Component> &component);

	/// Return a node from its unique identifier.
	std::shared_ptr<Node> GetNode(uint32_t uid) const;
	/// Return a  node from its name and parent.
	std::shared_ptr<Node> GetNode(const std::string &name) const;
	std::shared_ptr<Node> GetNode(const std::string &name, const std::shared_ptr<Node> &parent) const;

	/// Return a list of all nodes in the scene.
	const std::vector<std::shared_ptr<Node>> &GetNodes() const { return nodes; }
	/// Return a list of all child nodes of a node.
	std::vector<std::shared_ptr<Node>> GetNodeChildren(const Node &node) const;
	/// Return a list of nodes filtered by their name.
	std::vector<std::shared_ptr<Node>> GetNodes(const std::string &filter) const;

	/// Return a list of nodes with components implementing a specific aspect.
	std::vector<std::shared_ptr<Node>> GetNodesWithAspect(const std::string &aspect) const;

	/// Update the scene.
	void Update();
	virtual void Update(time_ns dt);
	/// Wait for the scene update to complete. Returns true when the scene update has completed.
	virtual bool WaitUpdate(bool blocking = true);

	/// Commit all scene writes.
	virtual void Commit();
	/// Wait for the scene commit to complete. Returns true when the scene commit has completed.
	virtual bool WaitCommit(bool blocking = true);

	/// Helper function to update and commit the scene, uses blocking waits.
	void UpdateAndCommitWaitAll(time_ns dt = 0);

	/// Return the scene serialization signal.
	const Signal<void(SceneSerializationState &)> &GetSerializationSignal() const { return serialization_signal; }
	Signal<void(SceneSerializationState &)> &GetSerializationSignal() { return serialization_signal; }

	/// Return the scene deserialization signal.
	const Signal<void(SceneDeserializationState &)> &GetDeserializationSignal() const { return deserialization_signal; }
	Signal<void(SceneDeserializationState &)> &GetDeserializationSignal() { return deserialization_signal; }

	/// Add aspect to scene.
	void AddComponent(std::shared_ptr<Component> component, int index = -1);
	/// Remove component from scene.
	void RemoveComponent(const std::shared_ptr<Component> &component);

	/// Return a list of all components in the scene.
	const std::vector<std::shared_ptr<Component>> &GetComponents() const { return components; }
	/// Return all components implementing a given aspect.
	std::vector<std::shared_ptr<Component>> GetComponents(const std::string &aspect) const;

	/// Return the first scene component implementing a specific aspect.
	std::shared_ptr<Component> GetComponent(const std::string &aspect) const {
		auto i = std::find_if(components.begin(), components.end(), [&aspect](const std::shared_ptr<Component> &c) { return c->GetAspect() == aspect; });
		return i != components.end() ? *i : nullptr;
	}

	/// Return the first scene component of a specific type.
	template <class T>
	std::shared_ptr<T> GetComponent() const {
		auto i = std::find_if(components.begin(), components.end(), [](const std::shared_ptr<Component> &c) {
			static auto type = g_type_registry.get().GetType<T>();
			return c->GetConcreteType() == type;
		});
		return i != components.end() ? std::static_pointer_cast<T>(*i) : nullptr;
	}

	/// Return true if the scene has a component implementing the specified aspect.
	bool HasAspect(const std::string &aspect) const;

	/// Return the scene (approximate) bounding box.
	MinMax GetMinMax() const;

	call_queue_mt commit_queue;

	void LockUpdate() { update_mutex.lock(); }
	void UnlockUpdate() { update_mutex.unlock(); }
	void LockCommit() { commit_mutex.lock(); }
	void UnlockCommit() { commit_mutex.unlock(); }

	/// Return the scene detailed phase identifier.
	uint_fast32_t GetPhaseDetailed() const { return phase_detailed; }

	Signal<void(Scene &, RenderSystem &)> &GetFrameClearedSignal() { return frame_cleared_signal; }
	Signal<void(Scene &, RenderSystem &)> &GetFrameCompleteSignal() { return frame_complete_signal; }
	Signal<void(Scene &, RenderSystem &)> &GetRenderablesDrawnSignal() { return renderables_drawn_signal; }

	time_ns force_dt{-1};

	std::vector<std::shared_ptr<AnimTake>> anim_takes; ///< All scene animation takes.

	void AddAnimTake(std::shared_ptr<AnimTake> take);

	/// Bind an animation take to the scene nodes, the returned animation objects must be manually applied.
	std::vector<std::shared_ptr<BoundAnim>> BindAnimTake(const std::string &name) const;
	/// Manage a set of bound anims through the scene animation system.
	std::shared_ptr<SceneBoundAnim> StartBoundAnims(const std::vector<std::shared_ptr<BoundAnim>> &bound_anims, AnimLoop loop = AnimOnce, time_ns t_start = time_undefined, float t_scale = 1.f);

	/// Bind an animation take to the scene nodes, the bound animation objects are managed by the scene.
	std::shared_ptr<SceneBoundAnim> StartAnimTake(const std::shared_ptr<AnimTake> &take, const std::vector<std::shared_ptr<Node>> &nodes, AnimLoop loop = AnimOnce, time_ns t_start = time_undefined, float t_scale = 1.f);
	std::shared_ptr<SceneBoundAnim> StartAnimTake(const std::string &name, const std::vector<std::shared_ptr<Node>> &nodes, AnimLoop loop = AnimOnce, time_ns t_start = time_undefined, float t_scale = 1.f);

	std::shared_ptr<SceneBoundAnim> StartAnimTake(const std::shared_ptr<AnimTake> &take, AnimLoop loop = AnimOnce, time_ns t_start = time_undefined, float t_scale = 1.f) {
		return StartAnimTake(take, nodes, loop, t_start, t_scale);
	}
	std::shared_ptr<SceneBoundAnim> StartAnimTake(const std::string &name, AnimLoop loop = AnimOnce, time_ns t_start = time_undefined, float t_scale = 1.f) {
		return StartAnimTake(name, nodes, loop, t_start, t_scale);
	}

	std::shared_ptr<SceneBoundAnim> StartGroupAnimTake(const std::shared_ptr<Group> &group, const std::string &name, AnimLoop loop = AnimOnce, time_ns t_start = time_undefined, float t_scale = 1.f);

	/// Bind an animation take to the scene nodes manually specifying its time range, the bound animation objects are managed by the scene.
	std::shared_ptr<SceneBoundAnim> StartAnimTakeRange(const std::shared_ptr<AnimTake> &take, const std::vector<std::shared_ptr<Node>> &nodes, time_ns t_range_start, time_ns t_range_end, AnimLoop loop = AnimOnce, time_ns t_start = time_undefined, float t_scale = 1.f);
	std::shared_ptr<SceneBoundAnim> StartAnimTakeRange(const std::string &name, const std::vector<std::shared_ptr<Node>> &nodes, time_ns t_range_start, time_ns t_range_end, AnimLoop loop = AnimOnce, time_ns t_start = time_undefined, float t_scale = 1.f);

	std::shared_ptr<SceneBoundAnim> StartAnimTakeRange(const std::shared_ptr<AnimTake> &take, time_ns t_range_start, time_ns t_range_end, AnimLoop loop = AnimOnce, time_ns t_start = time_undefined, float t_scale = 1.f) {
		return StartAnimTakeRange(take, nodes, t_range_start, t_range_end, loop, t_start, t_scale);
	}
	std::shared_ptr<SceneBoundAnim> StartAnimTakeRange(const std::string &name, time_ns t_range_start, time_ns t_range_end, AnimLoop loop = AnimOnce, time_ns t_start = time_undefined, float t_scale = 1.f) {
		return StartAnimTakeRange(name, nodes, t_range_start, t_range_end, loop, t_start, t_scale);
	}

	std::shared_ptr<SceneBoundAnim> StartGroupAnimTakeRange(const std::shared_ptr<Group> &group, const std::string &name, time_ns t_range_start, time_ns t_range_end, AnimLoop loop = AnimOnce, time_ns t_start = time_undefined, float t_scale = 1.f);

	void StopAnim(const std::shared_ptr<SceneBoundAnim> &anim);

	time_ns GetAnimT(const std::shared_ptr<SceneBoundAnim> &anim) const { return anim->t; }
	void SetAnimT(const std::shared_ptr<SceneBoundAnim> &anim, time_ns t);

	float GetAnimTScale(const std::shared_ptr<SceneBoundAnim> &anim) const { return float(anim->t_scale) / 256.f; }
	void SetAnimTScale(const std::shared_ptr<SceneBoundAnim> &anim, float t_scale);

	bool IsAnimLooped(const std::shared_ptr<SceneBoundAnim> &anim) const { return anim->t < anim->start || anim->t > anim->end; }

	size_t GetBoundAnimCount() const { return scene_bound_anims.size(); }

private:
	friend void SetSceneLoading(Scene &scene, bool is_loading);

	std::mutex update_mutex;
	std::mutex commit_mutex; ///< acquire this mutex to ensure that the scene is not in the commit phase

	std::atomic<Phase> phase_detailed{PhaseNone};

	std::atomic<bool> loading{false};

	Signal<void(SceneSerializationState &)> serialization_signal;
	Signal<void(SceneDeserializationState &)> deserialization_signal;

	std::vector<std::shared_ptr<SceneSystem>> systems;

	std::vector<std::shared_ptr<Group>> groups;

	uint32_t current_node_uid;
	std::vector<std::shared_ptr<Node>> nodes;

	void commit_AddNode(std::shared_ptr<Node> node);
	void commit_RemoveNode(const std::shared_ptr<Node> &node);

	void commit_Clear();

	std::vector<std::shared_ptr<Component>> components;

	void commit_AddComponent(std::shared_ptr<Component>, int index);
	void commit_RemoveComponent(const std::shared_ptr<Component> &component);

	void commit_AddComponentToSystems(std::shared_ptr<Component> component);
	void commit_RemoveComponentFromSystems(const std::shared_ptr<Component> &component);

	std::shared_ptr<Node> current_camera;
	void commit_SetCurrentCamera(std::shared_ptr<Node> node);

	void UpdateMouse();

	void OnRenderableSystemPreRenderFrame(RenderSystem &render_system);
	void OnRenderableSystemPostRenderFrame(RenderSystem &render_system);

	Signal<void(Scene &, RenderSystem &)> frame_cleared_signal; ///< Emitted after frame has been cleared
	Signal<void(Scene &, RenderSystem &)> frame_complete_signal; ///< Emitted after frame render has complete
	Signal<void(Scene &, RenderSystem &)> renderables_drawn_signal; ///< Emitted when all scene renderables have been drawn

	void OnRenderSystemFrameCleared(RenderSystem &render_system);
	void OnRenderSystemRenderablesDrawn(RenderSystem &render_system);
	void OnRenderSystemFrameComplete(RenderSystem &render_system);

	Signal<void(RenderSystem &)>::Connection render_system_frame_cleared_connection;
	Signal<void(RenderSystem &)>::Connection render_system_frame_complete_connection;
	Signal<void(RenderSystem &)>::Connection render_system_renderables_drawn_connection;

	void OnScriptSystemUpdateLogicContext(VM &vm, const ScriptLogicDlgts &dlgt);

	void commit_StartBoundAnims(const std::shared_ptr<SceneBoundAnim> &scene_bound_anim);

	void commit_StartAnimTakeRange(const std::shared_ptr<SceneBoundAnim> &anim, const std::shared_ptr<AnimTake> &take, const std::vector<std::shared_ptr<Node>> &nodes);
	void commit_StopAnim(const std::shared_ptr<SceneBoundAnim> &anim);

	void commit_SetAnimT(const std::shared_ptr<SceneBoundAnim> &anim, time_ns t);
	void commit_SetAnimTScale(const std::shared_ptr<SceneBoundAnim> &anim, float t_scale);

	void UpdateBoundAnims();
	std::future<void> update_bound_anims_future;

	std::vector<std::shared_ptr<SceneBoundAnim>> scene_bound_anims;
};

/// Create a new default scene.
std::shared_ptr<Scene> NewDefaultScene(std::shared_ptr<RenderSystem> render_system);
/// Load scene content.
bool LoadScene(Scene &scene, const std::string &path, std::shared_ptr<RenderSystem> render_system, std::vector<std::shared_ptr<Node>> *new_nodes = nullptr);
bool SaveScene(const Scene &scene, const std::string &path, std::shared_ptr<RenderSystem> render_system);

/// Set the scene is loading flag state.
void SetSceneLoading(Scene &scene, bool loading);

extern Signal<void(Scene &)> new_scene_signal;
extern Signal<void(Scene &)> delete_scene_signal;

extern Signal<void(Scene &)> scene_update_signal;

// Setup the default core system and components for a scene.
void SceneSetupCoreSystemsAndComponents(std::shared_ptr<Scene> scene, std::shared_ptr<RenderSystem> render_system);

} // namespace hg
