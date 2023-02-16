// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

// Support for std::vector reflection (for static C arrays see the StaticArrayInterfaceFactory)

#pragma once

#include "foundation/reflection.h"
#include <vector>

namespace hg {

template <typename T>
class StdVectorReadInterface : public ContainerReadInterface {
public:
	StdVectorReadInterface(const Type *type, const void *o) : ContainerReadInterface(type) { p_container = reinterpret_cast<const std::vector<T> *>(o); }

	size_t GetSize() const override { return p_container->size(); }
	TypeValue Get(size_t i) const override { return TypeValue(wrapped_type, p_container->data() + i); }
	const void *GetAddress(size_t i) const override { return p_container->data() + i; }

private:
	const std::vector<T> *p_container;
};

template <typename T>
class StdVectorWriteInterface : public ContainerWriteInterface {
public:
	StdVectorWriteInterface(const Type *type, void *o) : ContainerWriteInterface(type) { p_container = reinterpret_cast<std::vector<T> *>(o); }

	bool Reserve(size_t n) override {
		p_container->resize(n);
		return true;
	}
	void Set(size_t i, const TypeValue &v) override { (*p_container)[i] = v.Cast<T>(); }
	void *GetAddress(size_t i) const override { return p_container->data() + i; }

private:
	std::vector<T> *p_container;
};

template <typename T>
struct StdVectorInterfaceFactory : ContainerInterfaceFactory {
	explicit StdVectorInterfaceFactory(const Type *type) : ContainerInterfaceFactory(type) {}
	ContainerReadInterface *MakeReadInterface(const void *o) const override { return new StdVectorReadInterface<T>(wrapped_type, o); }
	ContainerWriteInterface *MakeWriteInterface(void *o) const override { return new StdVectorWriteInterface<T>(wrapped_type, o); }
};

} // namespace hg
