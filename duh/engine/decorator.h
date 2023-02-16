// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "foundation/reflection.h"

namespace hg {

class RenderGeometry;
struct Renderable;

/// Efficiently store and display a very large number (100Ks) of geometry instances
class Decorator : public std::enable_shared_from_this<Decorator>, public Component {
public:
	decl_component_defaults(Decorator, "Decorator", "Decorator");

	Decorator();

	comp_decl_member_get_sync_set_async_noimpl_novar(Decorator, std::shared_ptr<RenderGeometry>, Geometry);

	comp_decl_member_get_sync_set_async_noimpl_novar(Decorator, float, Range);
	comp_decl_member_get_sync_set_async_noimpl_novar(Decorator, bool, Decimate);
	comp_decl_member_get_sync_set_async_noimpl_novar(Decorator, float, DecimationRangeStart);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	/*
		Note: This function can safely be called during rendering but should not
		be called from two different threads at the same time.
	*/
	void AddInstance(const Matrix4 &world);

	ComponentState GetState() const override;

	std::shared_ptr<Renderable> GetRenderable() const override { return renderable; }

	mutable std::string data_path;

	bool SaveData(const std::string &path) const;
	bool LoadData(const std::string &path);

private:
	std::shared_ptr<Renderable> renderable;

	std::vector<Matrix4> queued_instances;
	void commit_AddInstance();
};

} // namespace hg
