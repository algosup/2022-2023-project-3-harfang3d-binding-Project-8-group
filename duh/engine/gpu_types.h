// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstdint>
#include <cstddef>

#if defined(ANDROID) || TARGET_OS_IPHONE
//#define EGL_HALF_FLOAT_SUPPORT
#endif

#ifdef EGL_HALF_FLOAT_SUPPORT
typedef unsigned short hfloat;
#define EGL_HFLOAT GL_HALF_FLOAT_OES
#define EGL_HFLOAT_SIZE 2
#else
typedef float hfloat;
#define EGL_HFLOAT GL_FLOAT
#define EGL_HFLOAT_SIZE 4
#endif

namespace hg {

enum IndexType : uint8_t {
	IndexUByte,
	IndexUShort,
	IndexUInt,
	IndexLast
};

extern size_t IndexTypeSize[IndexLast];

enum VertexType : uint8_t { // do not mistake for ShaderType
	VertexByte,
	VertexUByte,
	VertexShort,
	VertexUShort,
	VertexInt,
	VertexUInt,
	VertexFloat,
	VertexHalfFloat,
	VertexLast
};

extern size_t VertexTypeSize[VertexLast];

enum GpuPrimitiveType : uint8_t {
	GpuPrimitiveLine,
	GpuPrimitiveTriangle,
	GpuPrimitivePoint,
	GpuPrimitiveLast
};

/// Convert float to half-float.
hfloat FloatToHFloat(float f);
/// Convert float to half-float.
float HFloatToFloat(hfloat hf);

} // namespace hg
