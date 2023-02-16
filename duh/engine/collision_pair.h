// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/vector3.h"
#include <memory>

namespace hg {

class Node;

struct CollisionPair {
	CollisionPair() = default;
	CollisionPair(std::shared_ptr<Node> a_, std::shared_ptr<Node> b_) : a(std::move(a_)), b(std::move(b_)) {}

	std::shared_ptr<Node> a, b;

	struct Contact {
		Vector3 p, n;
	};

	uint8_t contact_count{0};
	Contact contact[4];
};

} // namespace hg
