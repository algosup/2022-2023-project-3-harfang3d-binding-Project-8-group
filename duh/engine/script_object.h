// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <memory>

namespace hg {

struct ScriptObject {
	struct Impl {
		virtual bool IsValid() const = 0;
		virtual bool IsNull() const = 0;
		virtual ~Impl() {}
	};

	ScriptObject() = default;
	explicit ScriptObject(std::shared_ptr<Impl> i) : impl(std::move(i)) {}
	ScriptObject(const ScriptObject &o) : impl(o.impl) {}

	ScriptObject &operator=(const ScriptObject &o) {
		impl = o.impl;
		return *this;
	}

	inline bool IsValid() const { return impl ? impl->IsValid() : false; }
	inline bool IsNull() const { return impl ? impl->IsNull() : false; }

	inline bool IsValidAndNonNull() const { return impl ? (impl->IsValid() && !impl->IsNull()) : false; }

	inline void Release() { impl.reset(); }

	std::shared_ptr<Impl> impl;
};

} // namespace hg
