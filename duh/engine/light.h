// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "engine/texture.h"
#include "engine/render_primitive.h"
#include "foundation/color.h"
#include "foundation/frustum.h"
#include "foundation/matrix4.h"
#include "foundation/matrix44.h"

namespace hg {

/// Light component
class Light : public std::enable_shared_from_this<Light>, public Component {
public:
	friend class LightSystem;

	decl_component_defaults(Light, "Light", "Light");

	Light();

	enum Model {
		ModelPoint,
		ModelLinear,
		ModelSpot,
		ModelLast
	};

	enum Shadow {
		ShadowNone,
		ShadowProjectionMap,
		ShadowMap
	};

	comp_decl_member_get_sync_set_async(Light, Model, Model, model);
	comp_decl_member_get_sync_set_async(Light, Shadow, Shadow, shadow);
	comp_decl_member_get_sync_set_async(Light, bool, ShadowCastAll, shadow_cast_all);
	comp_decl_member_get_sync_set_async_validated(Light, float, ShadowRange, shadow_range, ClampToZero);
	comp_decl_member_get_sync_set_async_validated(Light, float, ShadowBias, shadow_bias, ClampToZero);
	comp_decl_member_get_sync_set_async_validated(Light, Vector4, ShadowSplit, shadow_split, ClampVector4ToZeroOne);
	comp_decl_member_get_sync_set_async_validated(Light, float, ZNear, z_near, ZNearValidator);

	comp_decl_member_get_sync_noimpl_set_async_validated(Light, float, Range, range, ClampToZero);
	comp_decl_member_get_sync_set_async_validated(Light, float, ClipDistance, clip_distance, ClampToZero);

	comp_decl_member_get_sync_set_async(Light, Color, DiffuseColor, diffuse_color);
	comp_decl_member_get_sync_set_async(Light, Color, SpecularColor, specular_color);
	comp_decl_member_get_sync_set_async(Light, Color, ShadowColor, shadow_color);

	comp_decl_member_get_sync_set_async(Light, float, DiffuseIntensity, diffuse_intensity);
	comp_decl_member_get_sync_set_async(Light, float, SpecularIntensity, specular_intensity);
	comp_decl_member_get_sync_set_async_validated(Light, float, ConeAngle, cone_angle, ConeAngleValidator);
	comp_decl_member_get_sync_set_async_validated(Light, float, EdgeAngle, edge_angle, EdgeAngleValidator);

	comp_decl_member_get_sync_set_async(Light, std::shared_ptr<Texture>, ProjectionTexture, projection_texture);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	float GetCullRange() const;

	struct ShadowData {
		RenderPrimitiveLists prim_lists;

		fRect rect;
		FrustumPlanes frustum_planes;
		Matrix4 world;
		Matrix44 projection, projection_to_map;

		float slice_distance;
	};

	std::vector<ShadowData> shadow_data;

private:
	static float ZNearValidator(Light *l, float v) { return l->GetRange() > 0.001f ? Clamp(v, 0.001f, l->GetRange()) : v; }
	static float ConeAngleValidator(Light *l, float v) { return Clamp(v, 0.f, Pi * 0.5f - l->GetEdgeAngle()); }
	static float EdgeAngleValidator(Light *l, float v) { return Clamp(v, 0.f, Pi * 0.5f - l->GetConeAngle()); }
};

} // namespace hg
