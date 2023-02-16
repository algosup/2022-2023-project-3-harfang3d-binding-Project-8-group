// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/picture.h"
#include "foundation/declare_type_name.h"
#include "foundation/reflection.h"
#include <memory>

namespace hg {

struct TangentFrame;

class Geometry;
enum GeometryFlag : uint32_t;
struct GeometrySkin;

struct Polygon;

struct Shader;
enum ShaderType : uint8_t;
struct ShaderValue;

struct Material;

enum TextureUV : uint8_t;
enum TextureFilter : uint8_t;
struct TextureUnitConfig;

void RegisterCoreReflection(TypeRegistry &registry);

DeclareTypeName(Picture, "Picture");
DeclareTypeName(std::shared_ptr<Picture>, "std::shared_ptr<Picture>");

DeclareTypeName(TangentFrame, "TangentFrame");
DeclareTypeName(std::vector<TangentFrame>, "TangentFrameVector");

DeclareTypeName(Geometry, "Geometry");
DeclareTypeName(std::shared_ptr<Geometry>, "std::shared_ptr<Geometry>");
DeclareTypeName(GeometrySkin, "GeometrySkin");
DeclareTypeName(GeometryFlag, "GeometryFlag");

DeclareTypeName(Polygon, "Polygon");

DeclareTypeName(Shader, "Shader");
DeclareTypeName(std::shared_ptr<Shader>, "std::shared_ptr<Shader>");
DeclareTypeName(ShaderType, "ShaderType");
DeclareTypeName(ShaderValue, "ShaderValue");

DeclareTypeName(Material, "Material");
DeclareTypeName(std::shared_ptr<Material>, "std::shared_ptr<Material>");

DeclareTypeName(TextureUV, "TextureUV");
DeclareTypeName(TextureFilter, "TextureFilter");
DeclareTypeName(TextureUnitConfig, "TextureUnitConfig");

} // namespace hg
