// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace hg {

/// Instantiate shared objects from an identifier string
template <class T>
class SharedObjectFactory {
public:
	void Register(const std::string &name, std::function<std::shared_ptr<T>()> alloc) {
		auto &a_f = factories[name];
		a_f.alloc = std::move(alloc);
	}

	void Unregister(const std::string &name) {
		factories.erase(name);
	}

	std::shared_ptr<T> Instantiate() const {
		if (!factories.size())
			return nullptr;
		return factories.begin()->second.alloc();
	}

	std::shared_ptr<T> Instantiate(const std::string &name) const {
		auto i = factories.find(name);
		if (i == factories.end())
			return nullptr;
		return i->second.alloc();
	}

	std::vector<std::string> GetNames() const {
		std::vector<std::string> names;
		for (auto &i : factories)
			names.push_back(i.first);
		return names;
	}

	size_t GetCount() const { return factories.size(); }

private:
	struct alloc_free {
		std::function<std::shared_ptr<T>()> alloc;
	};

	std::map<std::string, alloc_free> factories;
};

} // namespace hg
