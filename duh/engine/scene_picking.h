// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/picture.h"
#include "engine/render_system.h"
#include "engine/scene.h"

namespace hg {

class ScenePicking {
public:
	explicit ScenePicking(std::shared_ptr<RenderSystem> render_system);

	/**
		@short Prepare picking queries.

		This function will queue rendering of the scene and capture output
		buffers for future picking queries.

		This function returns immediately and you should use the returned
		future object to determine when to start issuing queries.
	*/
	std::future<bool> Prepare(std::shared_ptr<Scene> scene, bool prepare_node_picking, bool prepare_world_picking);

	/**
		@short Scene node picking.

		This object must have successfully been prepared prior to calling this
		function.
	*/
	bool Pick(const Scene &scene, const iRect &rect, std::vector<std::shared_ptr<Node>> &nodes);
	bool Pick(const Scene &scene, int x, int y, std::vector<std::shared_ptr<Node>> &nodes) { return Pick(scene, iRect(x, y), nodes); }

	/*
		@short Scene world picking.

		This object must have successfully been prepared prior to calling this
		function.
	*/
	bool PickWorld(const Scene &scene, float x, float y, Vector3 &world_pos);

private:
	std::shared_ptr<RenderSystem> sys;

	std::shared_ptr<RenderTarget> fbo;
	std::shared_ptr<Texture> color, depth;

	Picture color_pic, depth_pic;

	struct PrepareState {
		iRect viewport;
		Matrix4 view_matrix;
		Matrix44 inv_projection_matrix;
	};

	PrepareState state;

	/// Synchronize internal objects state with the renderer.
	bool Capture(const Scene &scene, bool capture_color, bool capture_depth);
};

} // namespace hg
