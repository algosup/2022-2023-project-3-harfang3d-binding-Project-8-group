// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/reflection.h"

namespace hg {

enum SerializeFlags {
	SerializeDefault = 0x0,
	SerializeIgnoreTypeSerializer = 0x1,
};

bool SerializeEx(DocumentWriter &doc, const void *o, const Type *type, SerializationContext *ctx = nullptr, const std::string &name = std::string(), SerializeFlags flags = SerializeDefault);
bool Serialize(DocumentWriter &doc, const void *o, const Type *type, SerializationContext *ctx = nullptr, const std::string &name = std::string());

enum DeserializeFlags {
	DeserializeDefault = 0x0,
	DeserializeIgnoreTypeSerializer = 0x1,
};

bool DeserializeEx(DocumentReader &doc, void *o, const Type *type, DeserializationContext *ctx = nullptr, const std::string &name = std::string(), DeserializeFlags flags = DeserializeDefault);
bool Deserialize(DocumentReader &doc, void *o, const Type *type, DeserializationContext *ctx = nullptr, const std::string &name = std::string());

// This types mostly exist to reduce the SWIG binding complexity by hiding low-level details.
struct SerializationState {
	SerializationState(DocumentWriter *d = nullptr, const SerializationContext *c = nullptr) : doc(d), ctx(c) {}

	DocumentWriter *doc;
	const SerializationContext *ctx;
};

struct DeserializationState {
	DeserializationState(const DocumentReader *d = nullptr, DeserializationContext *c = nullptr) : doc(d), ctx(c) {}

	const DocumentReader *doc;
	const DeserializationContext *ctx;
};

} // namespace hg
