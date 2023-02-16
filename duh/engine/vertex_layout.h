// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/shader.h"
#include "engine/gpu_types.h"
#include <string.h>

namespace hg {

struct VertexLayout {
	typedef uint8_t Stride;

	static const int max_attribute = 8;

	struct Attribute { // 2 bytes
		Attribute() : semantic(VertexAttribute::LastSemantic), count(0), type(VertexByte), is_normalized(false) {}

		VertexAttribute::Semantic semantic : 8;
		uint8_t count : 4;
		VertexType type : 3;
		uint8_t is_normalized : 1;

		inline size_t ComputeSize() const {
			size_t size = count * VertexTypeSize[type];
			return (size & 1) ? size + 1 : size; // always align attributes
		}
	};

	inline bool operator==(const VertexLayout &layout) const { return memcmp(this, &layout, sizeof(VertexLayout)) == 0; }
	inline bool operator!=(const VertexLayout &layout) const { return !(*this == layout); }

	void Clear();
	bool AddAttribute(VertexAttribute::Semantic semantic, uint8_t count, VertexType type, bool is_normalized = false);
	void End();

	const Attribute *GetAttribute(VertexAttribute::Semantic semantic) const;

	Stride stride = 0;
	Attribute attr[max_attribute];
};

struct StdVertexLayout {
	StdVertexLayout();

	VertexLayout pos;
	VertexLayout pos_uv0;
	VertexLayout pos_uv0_color0;
};

extern StdVertexLayout std_vertex_layout;

} // namespace hg
