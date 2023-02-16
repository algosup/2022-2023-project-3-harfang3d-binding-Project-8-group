// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/declare_type_name.h"
#include "foundation/reflection.h"
#include <memory>

namespace hg {

struct Renderable;
class SurfaceShader;
class RenderMaterial;
class RenderGeometry;
class RenderSystem;
class RenderSystemAsync;

void RegisterRenderSystemReflection(TypeRegistry &registry);

DeclareTypeName(Renderable, "Renderable");
DeclareTypeName(std::shared_ptr<Renderable>, "std::shared_ptr<Renderable>");

DeclareTypeName(SurfaceShader, "SurfaceShader");
DeclareTypeName(std::shared_ptr<SurfaceShader>, "std::shared_ptr<SurfaceShader>");

DeclareTypeName(RenderMaterial, "RenderMaterial");
DeclareTypeName(std::shared_ptr<RenderMaterial>, "std::shared_ptr<RenderMaterial>");

DeclareTypeName(RenderGeometry, "RenderGeometry");
DeclareTypeName(std::shared_ptr<RenderGeometry>, "std::shared_ptr<RenderGeometry>");

DeclareTypeName(RenderSystem, "RenderSystem");
DeclareTypeName(std::shared_ptr<RenderSystem>, "std::shared_ptr<RenderSystem>");

DeclareTypeName(RenderSystemAsync, "RenderSystemAsync");
DeclareTypeName(std::shared_ptr<RenderSystemAsync>, "std::shared_ptr<RenderSystemAsync>");

} // namespace hg
