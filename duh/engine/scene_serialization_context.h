// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/document.h"
#include "foundation/reflection.h"
#include <memory>

namespace hg {

class RenderSystem;

/// Scene serialization context
struct SceneSerializationContext : SerializationContext {
	explicit SceneSerializationContext(std::shared_ptr<RenderSystem> render_system);

	const std::string &GetType() const override {
		static std::string type = "SceneSerializationContext";
		return type;
	}

	std::shared_ptr<RenderSystem> render_system;
};

class Node;
struct AnimTake;

enum SceneDeserializationFlag : uint32_t {
	SceneDeserializationFlagDefault = 0x0,

	SceneDeserializationFlagSkipSceneComponents = 0x1,
	SceneDeserializationFlagSkipSceneAnimations = 0x2,
	SceneDeserializationFlagSceneAnimationsInContextOnly = 0x4, // do not insert in Scene but deserialize animations in the context
};

/// Scene deserialization context
struct SceneDeserializationContext : DeserializationContext {
	explicit SceneDeserializationContext(std::shared_ptr<RenderSystem> render_system, size_t instance_depth = 0);

	const std::string &GetType() const override {
		static std::string type = "SceneDeserializationContext";
		return type;
	}

	SceneDeserializationFlag flags = SceneDeserializationFlagDefault;

	std::shared_ptr<RenderSystem> render_system;
	size_t instance_depth;

	std::map<uint32_t, std::shared_ptr<Node>> node_uid_map;
	std::vector<std::shared_ptr<Node>> GetNodes() const;

	std::vector<std::shared_ptr<AnimTake>> takes;
};

} // namespace hg
