// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/reflection.h"

namespace hg {

struct DocumentReader;
struct DocumentWriter;

struct MaterialTypeSerializer : TypeSerializer {
	virtual bool Deserialize(DocumentReader &doc, void *o, const Type *type, DeserializationContext *ctx = nullptr, const std::string &name = std::string()) override;
	virtual bool Serialize(DocumentWriter &doc, const void *o, const Type *type, SerializationContext *ctx = nullptr, const std::string &name = std::string()) override;
};

struct Material;

/// Load a material from path.
bool LoadMaterial(Material &mat, const std::string &path);
/// Save a material path.
bool SaveMaterial(const std::shared_ptr<hg::Material> &mat, const std::string &path, hg::DocumentFormat format = hg::DocumentFormatUnknown);

} // namespace hg
