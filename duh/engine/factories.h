// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/global_object.h"
#include "foundation/shared_object_factory.h"

namespace hg {

struct Mixer;
class Renderer;

class Movie;
class PhysicSystem;
struct FrameRenderer;

typedef SharedObjectFactory<Movie> MovieFactory;
typedef SharedObjectFactory<PhysicSystem> PhysicSystemFactory;
typedef SharedObjectFactory<Mixer> MixerFactory;
typedef SharedObjectFactory<Renderer> RendererFactory;
typedef SharedObjectFactory<FrameRenderer> FrameRendererFactory;

extern global_object<MovieFactory> g_movie_factory;
extern global_object<PhysicSystemFactory> g_physic_system_factory;
extern global_object<MixerFactory> g_mixer_factory;
extern global_object<RendererFactory> g_renderer_factory;
extern global_object<FrameRendererFactory> g_frame_renderer_factory;

} // namespace hg
