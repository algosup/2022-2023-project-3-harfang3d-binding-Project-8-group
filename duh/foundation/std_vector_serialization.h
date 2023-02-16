// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <vector>

#include "foundation/binary_data.h"

namespace hg {

struct DocumentReader;
struct DocumentWriter;

template <typename T>
bool DeserializeStdVector(DocumentReader &doc, std::vector<T> &c, const std::string &element_name, DeserializationContext *ctx = nullptr) {
	static auto type = g_type_registry.get().GetType<T>();
	if (!type)
		return false;

	auto count = doc.GetChildCount(element_name);
	c.resize(count);

	count = 0;
	if (doc.EnterFirstChild()) {
		do {
			if (!element_name.empty() && doc.GetScopeName() != element_name) // check element name
				continue;
			if (!Deserialize(doc, &c[count++], type, ctx, "value"))
				return false;
		} while (doc.EnterSibling());
	}
	return true;
}

template <typename T>
bool SerializeStdVector(DocumentWriter &doc, const std::vector<T> &c, const std::string &name, const std::string &element_name, SerializationContext *ctx = nullptr) {
	static auto type = g_type_registry.get().GetType<T>();
	if (!type)
		return false;

	if (!c.size())
		return true;

	WriteScopeGuard hierarchy_guard(doc, name);
	for (auto &cc : c) {
		WriteScopeGuard guard(doc, element_name);
		if (!Serialize(doc, &cc, type, ctx, "value"))
			return false;
	}
	return true;
}

template <typename T>
bool DeserializeElementFromBinary(T &v, const BinaryData &input) { return input.Read(v); }

template <typename T>
bool DeserializeStdVectorFromBinary(DocumentReader &doc, std::vector<T> &c, bool (*deserialize_element)(T &, const BinaryData &) = &DeserializeElementFromBinary<T>) {
	uint32_t size;
	if (!doc.Read("Count", size))
		return false;

	c.resize(size);

	BinaryData blob;
	if (!doc.Read("Data", blob))
		return false;

	for (auto &e : c)
		if (!deserialize_element(e, blob))
			return false;
	return true;
}

template <typename T>
bool SerializeElementAsBinary(const T &v, BinaryData &blob) {
	blob.Write(v);
	return true;
}

template <typename T>
bool SerializeStdVectorAsBinary(DocumentWriter &doc, const std::vector<T> &c, const std::string &name, bool (*serialize_element)(const T &, BinaryData &) = &SerializeElementAsBinary<T>) {
	if (!c.size())
		return true;

	WriteScopeGuard root_scope(doc, name);
	doc.Write("Count", uint32_t(c.size())); // element count

	BinaryData blob; // vertex data as binary blob
	for (auto &e : c)
		if (!serialize_element(e, blob))
			return false;
	return doc.Write("Data", blob);
}

} // namespace hg
