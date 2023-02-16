// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/reflection.h"

namespace hg {

struct DocumentReader;
struct DocumentWriter;

struct GeometryTypeSerializer : TypeSerializer {
	bool Deserialize(DocumentReader &doc, void *o, const Type *type, DeserializationContext *ctx = nullptr, const std::string &name = std::string()) override;
	bool Serialize(DocumentWriter &doc, const void *o, const Type *type, SerializationContext *ctx = nullptr, const std::string &name = std::string()) override;
};

class Geometry;

/// Load a geometry from path.
bool LoadGeometry(Geometry &geometry, const std::string &path);
/// Save a geometry to path.
bool SaveGeometry(const std::shared_ptr<hg::Geometry> &geo, const std::string &path, hg::DocumentFormat format = hg::DocumentFormatUnknown);

} // namespace hg
