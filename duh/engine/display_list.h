// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/gpu_buffer.h"
#include "engine/render_material.h"
#include "engine/vertex_layout.h"
#include "foundation/minmax.h"

namespace hg {

struct DisplayList {
	DisplayList() = default;
	DisplayList(uint32_t idx_count_, GpuBuffer *idx_, GpuBuffer *vtx_, const VertexLayout &vtx_layout_, const MinMax &minmax_, uint32_t material_,
		IndexType idx_type_ = IndexUShort)
		: idx_count(idx_count_), idx_type(idx_type_), idx(idx_), vtx(vtx_), vtx_layout(vtx_layout_), minmax(minmax_), material(material_) {}

	uint32_t idx_count{0};
	IndexType idx_type{IndexUShort};

	std::shared_ptr<GpuBuffer> idx;
	std::shared_ptr<GpuBuffer> vtx;
	VertexLayout vtx_layout;

	MinMax minmax;
	uint32_t material{0};
};

} // namespace hg
