// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/declare_type_name.h"
#include "foundation/task.h"
#include <memory>

namespace hg {

enum GpuBufferUsage {
	GpuBufferStatic,
	GpuBufferDynamic
};

enum GpuBufferType {
	GpuBufferIndex,
	GpuBufferVertex
};
	
struct GpuBuffer {};

class Renderer;

void GpuBufferDeleter(GpuBuffer *buffer, std::weak_ptr<Renderer> renderer_weak, task_affinity worker);

DeclareTypeName(GpuBuffer, "GpuBuffer");

} // namespace hg
