// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <map>
#include <string>
#include <vector>

namespace hg {

/// Instantiate objects from an identifier string
template <class T>
class Factory {
public:
	void Register(const char *name, void *(*factory)()) { factories[name] = factory; }

	T *Instantiate(const char *name) const {
		auto i = factories.find(name);
		return i != factories.end() ? reinterpret_cast<T *>((*i->second)()) : nullptr;
	}

	std::vector<std::string> GetNames() const {
		std::vector<std::string> names;
		for (auto &i : factories)
			names.push_back(i.first);
		return names;
	}

private:
	std::map<std::string, void *(*)()> factories;
};

} // namespace hg
