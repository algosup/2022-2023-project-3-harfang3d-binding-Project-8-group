// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/declare_type_name.h"
#include <memory>
#include <string>

namespace hg {

class Node;

/// Render pass
struct RenderPass {
	enum Pass : uint8_t {
		Opaque,
		Transparent,
		FramebufferDependent,
		Shadow,
		PassCount
	};

	enum Shader : uint8_t {
		Depth,

		DS_GBufferMRT4, ///< deferred MRT 4 buffers

		FS_Constant,
		FS_PointLight,
		FS_PointLightShadowMapping,
		FS_LinearLight,
		FS_LinearLightShadowMapping,
		FS_SpotLight,
		FS_SpotLightShadowMapping,
		FS_SpotLightProjection,
		FS_SpotLightProjectionShadowMapping,

		PP_NormalDepth,
		PP_Velocity,

		Picking, ///< node id rendering

		ShaderCount
	};

	RenderPass() = default;
	RenderPass(Pass g, Shader p) : pass(g), shader(p) {}

	inline bool operator==(const RenderPass &p) const { return pass == p.pass && shader == p.shader && light == p.light; }
	inline bool operator!=(const RenderPass &p) const { return !(*this == p); }

	Pass pass{PassCount};
	Shader shader{ShaderCount};

	std::shared_ptr<Node> light;
};

DeclareTypeName(RenderPass, "RenderPass");

} // namespace hg

namespace std {

string to_string(const hg::RenderPass &pass);
string to_string(hg::RenderPass::Pass pass_pass);
string to_string(hg::RenderPass::Shader pass_shader);

} // namespace std
