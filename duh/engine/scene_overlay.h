// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"

namespace hg {

class RenderSystem;

/**
	@short Scene overlay component.
	Base class for scene overlay components used to draw UI or custom debug
	elements.
*/
class SceneOverlay : public Component {
public:
	/// Called before the scene update starts.
	virtual void PreUpdate(RenderSystem &render_system) {}
	/// Draw the render overlay, this method is called from the render thread.
	virtual void Draw(RenderSystem &render_system) = 0;
	/// Called after the scene complete.
	virtual void Clear(RenderSystem &render_system) = 0;
};

} // namespace hg
