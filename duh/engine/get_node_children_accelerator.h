// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <map>
#include <memory>
#include <vector>

namespace hg {

class Node;

class GetNodeChildrenAccelerator {
public:
	void Build(const std::vector<std::shared_ptr<Node>> &nodes);
	std::vector<std::shared_ptr<Node>> GetChildren(const std::vector<std::shared_ptr<Node>> &nodes, const Node &node) const;

private:
	struct ChildrenInfo {
		uint32_t count;
		uint32_t start_index;
	};

	std::map<const Node *, uint32_t> node_to_idx;

	std::vector<ChildrenInfo> children_info;
	std::vector<uint32_t> children;
};

} // namespace hg
