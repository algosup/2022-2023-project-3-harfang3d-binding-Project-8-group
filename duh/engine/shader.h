// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/texture_unit_config.h"
#include <memory>
#include <string>
#include <vector>

namespace hg {

enum ShaderType : uint8_t {
	ShaderNoType,
	ShaderInt,
	ShaderUInt,
	ShaderFloat,
	ShaderVector2,
	ShaderVector3,
	ShaderVector4,
	ShaderMatrix3,
	ShaderMatrix4,
	ShaderTexture2D,
	ShaderTexture3D,
	ShaderTextureCube,
	ShaderTextureShadow,
	ShaderTextureExternal, // for OpenGL ES external OES
	ShaderLastType
};

bool TypeConsumesTextureUnit(ShaderType type);

enum ShaderTypePrecision : uint8_t {
	ShaderDefaultPrecision,
	ShaderLowPrecision,
	ShaderMediumPrecision,
	ShaderHighPrecision
};

struct VertexAttribute {
	enum Semantic : uint8_t {
		Position,
		Normal,
		UV0,
		UV1,
		UV2,
		UV3,
		Color0,
		Color1,
		Color2,
		Color3,
		Tangent,
		Bitangent,
		BoneIndex,
		BoneWeight,
		InstanceModelMatrix,
		InstancePreviousModelMatrix,
		InstancePickingId,
		LastSemantic
	};

	struct SemanticDesc {
		const char *name;
		ShaderType type;
		ShaderTypePrecision precision;
	};

	static const SemanticDesc semantic_desc[LastSemantic];
};

struct ShaderValue {
	ShaderValue() { iv[0] = iv[1] = iv[2] = iv[3] = 0; 
					iv[4] = iv[5] = iv[6] = iv[7] = 0;
					iv[8] = iv[9] = iv[10] = iv[11] = 0;
					iv[12] = iv[13] = iv[14] = iv[15] = 0;
	}

	union {
		int32_t iv[16];
		uint32_t uv[16];
		float fv[16];
	};

	std::string path;
	TextureUnitConfig tex_unit_cfg;
};

struct ShaderVariable {
	enum Semantic : uint8_t { // order must stay synchronized with semantic_desc declaration in shader.cpp
		Clock,
		Viewport,
		TechniqueIsForward,
		FxScale,
		InverseInternalResolution,
		InverseViewportSize,
		AmbientColor,
		FogColor,
		FogState,
		DepthBuffer,
		FrameBuffer,
		GBuffer0,
		GBuffer1,
		GBuffer2,
		GBuffer3,

		ViewVector,
		ViewPosition,
		ViewState,

		ModelMatrix,
		InverseModelMatrix,
		NormalMatrix,
		PreviousModelMatrix,
		ViewMatrix,
		InverseViewMatrix,
		ModelViewMatrix,
		NormalViewMatrix,
		ProjectionMatrix,
		ViewProjectionMatrix,
		ModelViewProjectionMatrix,
		InverseViewProjectionMatrix,
		InverseViewProjectionMatrixAtOrigin,
		PreviousViewMatrix,
		InversePreviousViewMatrix,

		LightState,
		LightDiffuseColor,
		LightSpecularColor,
		LightShadowColor,
		LightViewPosition,
		LightViewDirection,
		LightShadowMatrix,
		InverseShadowMapSize,
		LightShadowMap,
		LightPSSMSliceDistance,
		ViewToLightMatrix,
		LightProjectionMap,

		BoneMatrix,
		PreviousBoneMatrix,

		PickingId,

		TerrainHeightmap,
		TerrainHeightmapSize,
		TerrainSize,
		TerrainPatchOrigin,
		TerrainPatchSize,

		LastSemantic,

		CategoryGlobalStart = Clock,
		CategoryGlobalEnd = ViewVector,
		CategoryCameraStart = ViewVector,
		CategoryCameraEnd = ModelMatrix,
		CategoryTransformStart = ModelMatrix,
		CategoryTransformEnd = LightState,
		CategoryLightStart = LightState,
		CategoryLightEnd = BoneMatrix,
		CategorySkinningStart = BoneMatrix,
		CategorySkinningEnd = PickingId,
		CategoryNodeStart = PickingId,
		CategoryNodeEnd = TerrainHeightmap,
		CategoryTerrainStart = TerrainHeightmap,
		CategoryTerrainEnd = LastSemantic,
	};

	struct SemanticDesc {
		const char *name;
		ShaderType type;
		ShaderTypePrecision precision;
		uint8_t array_size;
	};

	static const SemanticDesc semantic_desc[LastSemantic];

	ShaderVariable();
	ShaderVariable(const ShaderVariable &var);

	std::string name;
	std::string hint;

	ShaderType type;
	ShaderTypePrecision precision;
	uint8_t array_size;

	std::unique_ptr<ShaderValue> value;
};

enum ShaderStageType : uint8_t {
	ShaderStageVertex, // do not reorder
	ShaderStagePixel,
	ShaderStageLast
};

struct ShaderVarying {
	enum Usage : uint8_t {
		In,
		Out
	};

	ShaderVarying(const char *name, ShaderType type, Usage vertex = Out, Usage pixel = In);

	std::string name;
	ShaderType type;

	Usage usage[ShaderStageLast];
};

/// High-level surface attributes
enum SurfaceAttribute {
	SurfaceNoAttribute = 0x00,

	SurfaceSkinned = 0x01,
	SurfaceIsTerrain = 0x02,
	SurfaceContinuousNormals = 0x04,
};

/// High-level surface draw state control flags
enum SurfaceDrawState {
	SurfaceDrawDefault = 0x00,

	SurfaceBlendAlpha = 0x01,
	SurfaceBlendAdditive = 0x02,
	SurfaceNoZWrite = 0x04,
	SurfaceNoZTest = 0x08,
	SurfaceAlphaTest = 0x10,
	SurfaceDoubleSided = 0x20,
	SurfaceCastShadow = 0x40,
	SurfaceReceiveShadow = 0x80,
	SurfaceNoLight = 0x100,
	SurfaceForceDepthFuncAlways = 0x200,
};

/// Shader source for a specific stage
struct ShaderStage {
	std::string global, source;
};

/// Shader variant for a specific platform or shader language
struct ShaderVariant {
	ShaderStage stages[ShaderStageLast];
	std::vector<std::unique_ptr<ShaderVarying>> varyings;
	std::vector<std::string> required_extensions;
};

/// Table of all externally accessible shader variables
struct ShaderVariableTable {
	std::vector<std::unique_ptr<ShaderVariable>> vars;
};

struct Shader : std::enable_shared_from_this<Shader> {
	Shader() : variables(new ShaderVariableTable) {}

	/// Clone a specific shader variant.
	void Clone(Shader &out, uint8_t variant_index) const;

	std::string name;

	std::shared_ptr<ShaderVariableTable> variables;
	std::vector<std::unique_ptr<ShaderVariant>> variants;

	uint16_t surface_draw_state{SurfaceCastShadow | SurfaceReceiveShadow};
	uint8_t surface_attributes{SurfaceNoAttribute};
	uint8_t alpha_threshold{128}; ///< normalized alpha threshold
};

std::string PreprocessShaderSource(const char *start, const char *end);

} // namespace hg

namespace std {

string to_string(hg::ShaderType type);
string to_string(hg::ShaderStageType type);
string to_string(hg::VertexAttribute::Semantic semantic);
string to_string(hg::ShaderVariable::Semantic semantic);

} // namespace std
