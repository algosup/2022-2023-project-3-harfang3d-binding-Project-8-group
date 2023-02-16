// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/declare_type_name.h"
#include "foundation/reflection.h"
#include <memory>

namespace hg {

class Renderer;
struct Texture;
struct RenderTarget;

void RegisterRendererReflection(TypeRegistry &registry);

DeclareTypeName(Renderer, "Renderer");
DeclareTypeName(std::shared_ptr<Renderer>, "std::shared_ptr<Renderer>");

DeclareTypeName(RenderTarget, "RenderTarget");
DeclareTypeName(std::shared_ptr<RenderTarget>, "std::shared_ptr<RenderTarget>");

DeclareTypeName(Texture, "Texture");
DeclareTypeName(std::shared_ptr<Texture>, "std::shared_ptr<Texture>");

} // namespace hg
