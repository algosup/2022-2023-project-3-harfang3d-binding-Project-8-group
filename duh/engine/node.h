// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/reflection.h"
#include "engine/component.h"
#include <vector>

namespace hg {

#define node_decl_flag_get_sync_set_async(N, V) \
private:                                        \
	bool V : 1;                                 \
	void commit_Set##N(bool v);                 \
                                                \
public:                                         \
	bool Get##N() const { return V; }           \
	void Set##N(bool v);

#define node_impl_set_async(N)                                                     \
	void Node::Set##N(bool v) {                                                    \
		if (auto s = scene.lock())                                                 \
			s->commit_queue.queue<void>(std::bind(&Node::commit_Set##N, this, v)); \
		else                                                                       \
			commit_Set##N(v);                                                      \
	}

/// Scene node
class Node : public std::enable_shared_from_this<Node> {
public:
	explicit Node();
	explicit Node(const std::string &name);
	virtual ~Node();

	static const uint32_t InvalidUid = 0;

	/// Return the node scene.
	std::shared_ptr<Scene> GetScene() const { return scene.lock(); }
	/// Return the node uid.
	uint32_t GetUid() const { return uid; }

	/// Return the node name.
	const std::string &GetName() const { return name; }
	/// Set the node name.
	void SetName(const std::string &name);

	/// Add aspect to entity.
	void AddComponent(std::shared_ptr<Component> component, int index = -1);
	/// Remove aspect from entity.
	void RemoveComponent(const std::shared_ptr<Component> &component);

	/// Return a list of the node components.
	const std::vector<std::shared_ptr<Component>> &GetComponents() const { return components; }
	/// Return a list of the node components implementing a given aspect.
	std::vector<std::shared_ptr<Component>> GetComponents(const std::string &aspect) const;

	/// Return the first node component implementing a specific aspect.
	std::shared_ptr<Component> GetComponent(const std::string &aspect) const {
		auto i = std::find_if(components.begin(), components.end(), [&aspect](const std::shared_ptr<Component> &c) { return c->GetAspect() == aspect; });
		return i != components.end() ? *i : nullptr;
	}

	/// Return the first node component of a specific type.
	template <class T>
	std::shared_ptr<T> GetComponent() const {
		auto i = std::find_if(components.begin(), components.end(), [](const std::shared_ptr<Component> &c) {
			static auto type = g_type_registry.get().GetType<T>();
			return c->GetConcreteType() == type;
		});
		return i != components.end() ? std::static_pointer_cast<T>(*i) : nullptr;
	}

	/// Test if the node has a component implementing a specific aspect.
	bool HasAspect(const std::string &aspect) const { return bool(GetComponent(aspect)); }

	/// Return true if the node and all its components are ready, false otherwise.
	bool IsReady() const;

	/// Return the node that instantiated this node.
	const std::shared_ptr<Node> &GetInstantiatedByNode() const;
	/// Flag this node as instantiated by a node.
	void SetInstantiatedBy(std::shared_ptr<Node> node);
	/// Return the scene that instantiated this node.
	const std::shared_ptr<Scene> &GetInstantiatedByScene() const;
	/// Flag this node as instantiated by a scene.
	void SetInstantiatedBy(std::shared_ptr<Scene> scene);

	bool IsInstantiated() const { return instantiated_by_node || instantiated_by_scene; }

	std::shared_ptr<Node> GetNode(uint32_t uid) const;
	std::shared_ptr<Node> GetNode(const std::string &name) const;
	std::shared_ptr<Node> GetNode(const std::string &name, const std::shared_ptr<Node> &parent) const;

	node_decl_flag_get_sync_set_async(Enabled, enabled);
	node_decl_flag_get_sync_set_async(IsStatic, is_static);
	node_decl_flag_get_sync_set_async(DoNotSerialize, do_not_serialize);
	node_decl_flag_get_sync_set_async(DoNotInstantiate, do_not_instantiate);
	node_decl_flag_get_sync_set_async(UseForNavigation, use_for_navigation);

private:
	friend void NodeSetInstantiatedBy(Node &node, std::shared_ptr<Node> by);
	friend void NodeSetInstantiatedBy(Node &node, std::shared_ptr<Scene> by);
	friend void NodeSetScene(Node &node, std::shared_ptr<Scene> scene);
	friend void NodeSetUid(Node &node, uint32_t uid);

	std::weak_ptr<Scene> scene;

	uint32_t uid{InvalidUid};
	std::string name;

	std::vector<std::shared_ptr<Component>> components;

	uint8_t cidx_transform{0xff};
	uint8_t cidx_camera{0xff};
	uint8_t cidx_object{0xff};
	uint8_t cidx_light{0xff};

	void BindComponentAccessors();

	void commit_SetName(const std::string &name);

	void commit_AddComponent(std::shared_ptr<Component> component, uint32_t index);
	void commit_RemoveComponent(const std::shared_ptr<Component> &component);

	std::shared_ptr<Node> instantiated_by_node;
	void commit_SetInstantiatedByNode(std::shared_ptr<Node> node);
	std::shared_ptr<Scene> instantiated_by_scene;
	void commit_SetInstantiatedByScene(std::shared_ptr<Scene> scene);
};

// fast path for critical/often used component types
class Transform;
class Camera;
class Object;
class Light;

template <>
std::shared_ptr<Transform> Node::GetComponent<Transform>() const;
template <>
std::shared_ptr<Camera> Node::GetComponent<Camera>() const;
template <>
std::shared_ptr<Object> Node::GetComponent<Object>() const;
template <>
std::shared_ptr<Light> Node::GetComponent<Light>() const;

// internal helpers
void NodeSetInstantiatedBy(Node &node, std::shared_ptr<Node> by);
void NodeSetInstantiatedBy(Node &node, std::shared_ptr<Scene> by);
void NodeSetScene(Node &node, std::shared_ptr<Scene> scene);
void NodeSetUid(Node &node, uint32_t uid);

} // namespace hg
