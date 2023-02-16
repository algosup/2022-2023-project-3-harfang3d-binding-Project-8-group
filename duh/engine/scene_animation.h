// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <memory>
#include <vector>

namespace hg {

class Node;

struct Anim;
struct BoundAnim;

/// Bound an animation to a node.
std::shared_ptr<BoundAnim> BindNodeAnim(const Node &node, const Anim &anim);
/// Bound a set of animations to a set of nodes.
std::vector<std::shared_ptr<BoundAnim>> BindNodesAnims(const std::vector<std::shared_ptr<Node>> &nodes, const std::vector<std::shared_ptr<Anim>> &anims);

} // namespace hg
