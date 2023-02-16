// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/reflection.h"
#include "foundation/type_to_document.h"
#include <memory>
#include <vector>

namespace hg {

class Scene;

/// Register serialization for all scene types.
void RegisterSceneSerialization(TypeRegistry &registry);

struct SceneSerializationState : SerializationState {
	SceneSerializationState(DocumentWriter *d = nullptr, Scene *s = nullptr, SerializationContext *c = nullptr) : SerializationState(d, c), scene(s) {}
	Scene *scene;
};

struct SceneDeserializationState : DeserializationState {
	SceneDeserializationState(const DocumentReader *d = nullptr, Scene *s = nullptr, DeserializationContext *c = nullptr) : DeserializationState(d, c), scene(s) {}
	Scene *scene;
};

class Node;

/// Serialize a selection of nodes.
bool SerializeNodeSelection(DocumentWriter &doc, const std::vector<std::shared_ptr<Node>> &nodes, SerializationContext *ctx);

struct AnimTake;

/// Serialize animation take.
bool SerializeAnimationTake(DocumentWriter &doc, AnimTake &take, const void *ctx);
/// Serialize animation takes.
bool SerializeAnimationTakes(DocumentWriter &doc, const std::vector<std::shared_ptr<AnimTake>> &takes, const void *ctx);

} // namespace hg
