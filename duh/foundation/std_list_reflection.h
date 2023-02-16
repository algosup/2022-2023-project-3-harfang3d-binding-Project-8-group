// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

// Support for std::list reflection

#pragma once

#include "foundation/reflection.h"
#include <list>

namespace hg {

template <typename T>
class StdListReadInterface : public ContainerReadInterface {
public:
	StdListReadInterface(const Type *wrapped_type, const void *o) : ContainerReadInterface(wrapped_type) { p_list = reinterpret_cast<const std::list<T> *>(o); }

	size_t GetSize() const override { return p_list->size(); }

	TypeValue Get(size_t idx) const override {
		auto i = p_list->begin();
		std::advance(i, idx);
		return TypeValue(wrapped_type, &(*i));
	}

	const void *GetAddress(size_t idx) const override {
		auto i = p_list->begin();
		std::advance(i, idx);
		return &(*i);
	}

private:
	const std::list<T> *p_list;
};

template <typename T>
class StdListWriteInterface : public ContainerWriteInterface {
public:
	StdListWriteInterface(const Type *wrapped_type, void *o) : ContainerWriteInterface(wrapped_type) { p_list = reinterpret_cast<std::list<T> *>(o); }

	bool Reserve(size_t n) override {
		for (uint32_t i = 0; i < n; ++i)
			p_list->push_back(T());
		return true;
	}

	void Set(size_t idx, const TypeValue &v) override {
		auto i = p_list->begin();
		std::advance(i, idx);
		*i = v.Cast<T>();
	}

	void *GetAddress(size_t idx) const override {
		auto i = p_list->begin();
		std::advance(i, idx);
		return &(*i);
	}

private:
	std::list<T> *p_list;
};

template <typename T>
struct StdListInterfaceFactory : ContainerInterfaceFactory {
	StdListInterfaceFactory(const Type *wrapped_type) : ContainerInterfaceFactory(wrapped_type) {}
	ContainerReadInterface *MakeReadInterface(const void *o) const override { return new StdListReadInterface<T>(wrapped_type, o); }
	ContainerWriteInterface *MakeWriteInterface(void *o) const override { return new StdListWriteInterface<T>(wrapped_type, o); }
};

} // namespace hg
