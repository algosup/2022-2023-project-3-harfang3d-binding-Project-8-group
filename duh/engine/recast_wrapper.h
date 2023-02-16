// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/minmax.h"
#include "foundation/vector3.h"
#include <array>
#include <memory>
#include <vector>

class dtNavMesh;
class dtNavMeshQuery;

namespace hg {

class Geometry;

/// Navigation path
struct NavigationPath {
	std::vector<Vector3> point;
};

/// Navigation layer
struct NavigationLayer {
	bool operator==(const NavigationLayer &layer) const { return radius == layer.radius && height == layer.height && slope == layer.slope; }
	bool operator!=(const NavigationLayer &layer) const { return !(*this == layer); }
	float radius{0.6f}, height{0.5f}, slope{0.6f};
};

/// Navigation configuration
struct NavigationConfig {
	NavigationConfig();

	bool operator==(const NavigationConfig &config) const {
		if (layers.size() != config.layers.size())
			return false;

		for (size_t i = 0; i < layers.size(); ++i)
			if (layers[i] != config.layers[i])
				return false;

		return true;
	}

	std::vector<NavigationLayer> layers;
};

/// Navigation geometry
class NavigationGeometry {
public:
	struct Layer {
		Layer();
		~Layer();

		dtNavMesh *mesh;
		dtNavMeshQuery *query;
	};

	const NavigationConfig &GetConfig() const { return config; }
	const std::vector<Layer> &GetLayers() const { return layers; }

	const MinMax &GetMinMax() const { return minmax; }

private:
	friend std::shared_ptr<NavigationGeometry> CreateNavigationGeometry(const Geometry &geo, const NavigationConfig &cfg);

	NavigationConfig config;
	std::vector<Layer> layers;

	MinMax minmax;
};

/// Create a navigation geometry.
std::shared_ptr<NavigationGeometry> CreateNavigationGeometry(const Geometry &geo, const NavigationConfig &cfg);

/// Perform a path query.
bool FindNavigationPathTo(const Vector3 &from, const Vector3 &to, dtNavMeshQuery *query, NavigationPath &path);

} // namespace hg
