// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/call_queue_mt.h"
#include "foundation/vector4.h"
#include <memory>
#include <string>

namespace hg {

class Type;

class call_queue_mt;

class Node;
class Scene;
class SceneSystem;

struct Renderable;
struct FrameRenderer;

enum ComponentState : uint8_t {
	NotReady,
	Ready,
	Failed
};

/**
	@short Component base
	A component can only be registered in a single system.
*/
class Component {
public:
	Component() : enabled(true), show_in_editor(true), do_not_serialize(false) {}
	virtual ~Component() = default;

	/// Return the component concrete implementation type.
	Type *GetConcreteType() const { return concrete_type; }

	/// Get the node system this component is registered into.
	std::shared_ptr<SceneSystem> GetSceneSystem() const { return system.lock(); }
	/// Get the scene this component is registered into.
	std::shared_ptr<Scene> GetScene() const { return scene.lock(); }
	/// Get the node this component is registered into.
	std::shared_ptr<Node> GetNode() const { return node.lock(); }

	/// Return true if that node is registered in a scene or node.
	bool IsAssigned() const { return !scene.expired() || !node.expired(); }

	/// Return true if the component is enabled.
	bool GetEnabled() const { return enabled; }
	/// Set the component as active or inactive.
	void SetEnabled(bool v) { commit_queue ? commit_queue->queue<void>(std::bind(&Component::commit_SetEnabled, this, v)) : commit_SetEnabled(v); }

	/// Return the component renderable interface if it implements it.
	virtual std::shared_ptr<Renderable> GetRenderable() const { return nullptr; }
	/// Return the frame renderer interface if the component implements it.
	virtual std::shared_ptr<FrameRenderer> GetFrameRenderer() const { return nullptr; }

	/// Return the status of the component
	virtual ComponentState GetState() const { return Ready; }

	/// Update component in the system it belongs to.
	void UpdateComponent();

	/// Component name.
	virtual const std::string &GetName() const = 0;
	/// Component aspect.
	virtual const std::string &GetAspect() const = 0;

	/// Return true if the component should not be serialized.
	bool GetDoNotSerialize() const { return do_not_serialize; }
	/// Prevent the component from being serialized.
	void SetDoNotSerialize(bool v) { commit_queue ? commit_queue->queue<void>(std::bind(&Component::commit_SetDoNotSerialize, this, v)) : commit_SetDoNotSerialize(v); }

	/// Return true if the component should not be serialized.
	bool GetShowInEditor() const { return show_in_editor; }
	/// Prevent the component from being serialized.
	void SetShowInEditor(bool v) { commit_queue ? commit_queue->queue<void>(std::bind(&Component::commit_SetShowInEditor, this, v)) : commit_SetShowInEditor(v); }

	/// Return the scene this component is registered into (note: either the scene or the scene of the node it is attached to).
	std::shared_ptr<Scene> GetRegisteredInScene() const;

	virtual std::shared_ptr<Component> shared_from_base() = 0;

protected:
	Type *concrete_type{nullptr};

	call_queue_mt *commit_queue{nullptr};

private:
	friend void ComponentSetSceneSystem(Component &component, const std::shared_ptr<SceneSystem> &system);
	friend void ComponentSetScene(Component &component, const std::shared_ptr<Scene> &scene);
	friend void ComponentSetNode(Component &component, const std::shared_ptr<Node> &node);

	std::weak_ptr<SceneSystem> system;
	std::weak_ptr<Scene> scene;
	std::weak_ptr<Node> node;

	bool enabled : 1;
	bool show_in_editor : 1;
	bool do_not_serialize : 1;

private:
	void commit_SetEnabled(bool enabled);
	void commit_SetShowInEditor(bool show) { show_in_editor = show; }
	void commit_SetDoNotSerialize(bool do_not) { do_not_serialize = do_not; }
};

template <typename C, typename T>
T DefaultValidator(C *, const T &v) { return v; }

template <typename C, typename T>
T ClampToZero(C *, const T &v) { return v > 0 ? v : 0; }
template <typename C, typename T>
T ClampToZeroOne(C *, const T &v) { return Clamp<T>(v, 0, 1); }

template <typename C>
Vector4 ClampVector4ToZeroOne(C *, const Vector4 &v) {
	return Vector4(Clamp(v.x, 0.f, 1.f), Clamp(v.y, 0.f, 1.f), Clamp(v.z, 0.f, 1.f), Clamp(v.w, 0.f, 1.f));
}

} // namespace hg

//
#define comp_decl_member(C, T, N) \
public:                           \
	const T Get##N() const;       \
	void Set##N(const T &v);

#define comp_decl_base_member(C, T, N)  \
public:                                 \
	virtual const T Get##N() const = 0; \
	virtual void Set##N(const T &v) = 0;

#define comp_decl_member_get_sync_set_async_validated(C, T, N, V, VALIDATOR) \
private:                                                                     \
	T V;                                                                     \
	void commit_Set##N(const T &v) { V = VALIDATOR(this, v); }               \
                                                                             \
public:                                                                      \
	const T Get##N() const { return V; }                                     \
	void Set##N(const T &v);

#define comp_decl_member_get_sync_noimpl_set_async_validated(C, T, N, V, VALIDATOR) \
private:                                                                            \
	T V;                                                                            \
	void commit_Set##N(const T &v) { V = VALIDATOR(this, v); }                      \
                                                                                    \
public:                                                                             \
	const T Get##N() const;                                                         \
	void Set##N(const T &v);

#define comp_decl_member_get_sync_set_async(C, T, N, V) comp_decl_member_get_sync_set_async_validated(C, T, N, V, DefaultValidator)

#define comp_decl_member_get_sync_set_async_noimpl_novar(C, T, N) \
private:                                                          \
	void commit_Set##N(const T &v);                               \
                                                                  \
public:                                                           \
	const T Get##N() const;                                       \
	void Set##N(const T &v);

#define comp_decl_member_get_sync_set_async_noimpl(C, T, N, V) \
private:                                                       \
	T V;                                                       \
	void commit_Set##N(const T &v);                            \
                                                               \
public:                                                        \
	const T Get##N() const { return V; }                       \
	void Set##N(const T &v);

#define comp_impl_set_async_fn(C, T, N) \
	void C::Set##N(const T &v) { commit_queue ? commit_queue->queue<void>(std::bind(&C::commit_Set##N, this, v)) : commit_Set##N(v); }

#define comp_decl_member_compatibility(C, T, N) \
public:                                         \
	const T Get##N() const;                     \
	void Set##N(const T &v);

//
#define comp_decl_shared_obj_member_get_sync_set_async(C, T, N, V) \
private:                                                           \
	std::shared_ptr<T> V;                                          \
	void commit_Set##N(const T *v) { V = v; }                      \
                                                                   \
public:                                                            \
	const T *Get##N() const { return V; }                          \
	void Set##N(const T *v);

#define comp_decl_shared_obj_member_get_sync_set_async_noimpl(C, T, N, V) \
private:                                                                  \
	std::shared_ptr<T> V;                                                 \
	void commit_Set##N(const T *v);                                       \
                                                                          \
public:                                                                   \
	const T *Get##N() const { return V; }                                 \
	void Set##N(const T *v);

//
#define decl_component_base_defaults(NAME, ASPECT)  \
public:                                             \
	const std::string &GetName() const override {   \
		static std::string name = NAME;             \
		return name;                                \
	}                                               \
                                                    \
	const std::string &GetAspect() const override { \
		static std::string aspect = ASPECT;         \
		return aspect;                              \
	}

#define decl_component_defaults(CLASS, NAME, ASPECT) \
public:                                              \
	decl_component_base_defaults(NAME, ASPECT)       \
                                                     \
		static std::shared_ptr<Component>            \
		Make() {                                     \
		return std::make_shared<CLASS>();            \
	}
