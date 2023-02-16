// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace hg {

template <typename T>
class ResourceCache {
public:
	/// Purge resource cache, remove resources with no external reference.
	size_t Purge() {
		std::lock_guard<std::mutex> guard(access);
		auto before = cache.size();
		cache.erase(std::remove_if(cache.begin(), cache.end(), [](const std::shared_ptr<T> &s) { return s.use_count() == 1; }), cache.end());
		return before - cache.size();
	}

	/// Clear resource cache, unconditionally remove all resources.
	void Clear() {
		std::lock_guard<std::mutex> guard(access);
		cache.clear();
	}

	/// Retrieve a resource from the cache.
	std::shared_ptr<T> Get(const std::string &name) const {
		std::lock_guard<std::mutex> guard(access);
		auto i = std::find_if(cache.begin(), cache.end(), [&name](const std::shared_ptr<T> &t) { return t->GetName() == name; });
		return i != cache.end() ? *i : nullptr;
	}

	/// Test cache for the presence of a resource.
	bool Has(const std::string &name) const { return bool(Get(name)); }

	/// Test cache for the presence of a resource, create it if missing.
	std::shared_ptr<T> HasOrMake(const std::string &name, bool &had, std::function<std::shared_ptr<T>(const std::string &)> make) {
		had = true;

		std::lock_guard<std::mutex> guard(access);
		auto i = std::find_if(cache.begin(), cache.end(), [&name](const std::shared_ptr<T> &t) { return t->GetName() == name; });
		if (i != cache.end())
			return *i;

		had = false;
		cache.emplace_back(make(name));
		return cache.back();
	}

	/// Add a resource to the cache.
	void Add(std::shared_ptr<T> t) {
		std::lock_guard<std::mutex> guard(access);
		cache.emplace_back(std::move(t));
	}

	/// Return the cache content.
	const std::vector<std::shared_ptr<T>> &GetContent() const { return cache; }

private:
	mutable std::mutex access;
	std::vector<std::shared_ptr<T>> cache;
};

} // namespace hg
