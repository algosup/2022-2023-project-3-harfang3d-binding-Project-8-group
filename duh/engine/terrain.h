// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "foundation/vector2.h"
#include "foundation/vector3.h"
#include <memory>
#include <string>

namespace hg {

class Picture;

struct Texture;
struct Renderable;
struct TerrainData;

class SurfaceShader;

/// Terrain renderable
class Terrain : public std::enable_shared_from_this<Terrain>, public Component {
public:
	static const int terrain_max_recursion;

	Terrain();

	decl_component_defaults(Terrain, "Terrain", "Terrain");

	comp_decl_member_get_sync_set_async_noimpl(Terrain, std::string, Heightmap, heightmap);
	comp_decl_member_get_sync_set_async_noimpl(Terrain, iVector2, HeightmapResolution, heightmap_resolution);
	comp_decl_member_get_sync_set_async_noimpl(Terrain, uint32_t, HeightmapBpp, heightmap_bpp);

	comp_decl_member_get_sync_set_async_noimpl(Terrain, Vector3, Size, size); // in meters

	comp_decl_member_get_sync_set_async_noimpl(Terrain, std::string, SurfaceShader, surface_shader);

	comp_decl_member_get_sync_set_async_validated(Terrain, int, PatchSubdivisionThreshold, patch_subdv_threshold, PatchSubdivisionThresholdValidator); // maximum normalized size of a patch subdivision on screen
	comp_decl_member_get_sync_set_async_validated(Terrain, int, MaxRecursion, max_recursion, MaxRecursionValidator); // maximum patch recursion
	comp_decl_member_get_sync_set_async_validated(Terrain, float, MinPrecision, min_precision, MinPrecisionValidator); // minimum patch subdivision size in meters

	comp_decl_member_get_sync_set_async(Terrain, bool, Wireframe, wireframe);

	std::shared_ptr<Renderable> GetRenderable() const override { return renderable; }
	std::shared_ptr<SurfaceShader> GetTerrainSurfaceShader() const;

	ComponentState GetState() const override;

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

private:
	std::shared_ptr<Renderable> renderable;

	static int PatchSubdivisionThresholdValidator(Terrain *terrain, int value);
	static int MaxRecursionValidator(Terrain *terrain, int value);
	static float MinPrecisionValidator(Terrain *terrain, float value);
};

/// Load a terrain heightmap to a picture object.
bool LoadHeightmap(Picture &heightmap_pic, const char *path, uint32_t width, uint32_t height, uint32_t bpp);
/// Normalize an heightmap picture.
bool NormalizeHeightmap(Picture &heightmap, uint32_t invalid_value);
/// Save heightmap picture.
bool SaveHeightmap(const Picture &heightmap, const char *path);

} // namespace hg
