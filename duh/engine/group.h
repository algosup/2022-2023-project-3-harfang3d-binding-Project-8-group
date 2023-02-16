// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace hg {

struct AnimTake;
class Node;

/// Group of nodes
class Group {
public:
	const std::string &GetName() const { return name; }
	void SetName(const std::string &_name) { name = _name; }

	bool IsReady() const;

	inline const std::vector<std::shared_ptr<Node>> &GetNodes() const { return nodes; }
	inline const std::vector<std::shared_ptr<Group>> &GetGroups() const { return groups; }
	inline const std::vector<std::shared_ptr<AnimTake>> &GetAnimTakes() const { return takes; }

	void AddNode(std::shared_ptr<Node> node);
	void RemoveNode(const std::shared_ptr<Node> &node);
	bool IsMember(const std::shared_ptr<Node> &node) const;

	std::shared_ptr<Node> GetNode(uint32_t uid) const;
	std::shared_ptr<Node> GetNode(const std::string &name) const;
	std::shared_ptr<Node> GetNode(const std::string &name, const std::shared_ptr<Node> &parent) const;

	void AddAnimTake(std::shared_ptr<AnimTake> take);
	void RemoveAnimTake(const std::shared_ptr<AnimTake> &take);

	std::shared_ptr<AnimTake> GetAnimTake(const std::string &name) const;

	void AddGroup(std::shared_ptr<Group> group);
	void RemoveGroup(const std::shared_ptr<Group> &group);
	bool IsMember(const std::shared_ptr<Group> &group) const;

	std::shared_ptr<Group> GetGroup(const std::string &name) const;

	/// Append a group content to this group.
	void AppendGroup(const Group &group);

private:
	std::string name;

	std::vector<std::shared_ptr<Node>> nodes;
	std::vector<std::shared_ptr<Group>> groups;
	std::vector<std::shared_ptr<AnimTake>> takes;
};

} // namespace hg
