// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "foundation/minmax.h"
#include "foundation/math.h"

namespace hg {

struct Skeleton;
struct Renderable;

class Transform;
class RenderGeometry;

/// Display a renderable when added to a Node with a Transform component
class Object : public std::enable_shared_from_this<Object>, public Component {
public:
	decl_component_defaults(Object, "Object", "Object");

	Object();
	~Object();

	comp_decl_member_get_sync_set_async_noimpl(Object, std::shared_ptr<RenderGeometry>, Geometry, geometry);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	ComponentState GetState() const override;

	/// Return the component renderable interface if it implements it.
	std::shared_ptr<Renderable> GetRenderable() const override { return renderable; }

	/// Return the object minmax.
	MinMax GetLocalMinMax() const;

	const std::shared_ptr<Skeleton> &GetSkeleton() const { return skeleton; }

	/// Return the binding bone matrix.
	bool GetBindMatrix(uint32_t index, Matrix4 &bind_matrix) const;
	/// Update skeleton matrices.
	void UpdateSkeleton(const Transform &transform);

	void AllocateSkeleton(uint32_t bone_count); // FIXME should be asynchronous
	void FreeSkeleton(); // FIXME should be asynchronous

	bool BindBone(uint32_t index, std::shared_ptr<Node> bone); // FIXME should be asynchronous

	bool HasSkeleton() const;
	bool IsSkinBound() const;

	uint32_t GetBoneCount() const;
	const std::shared_ptr<Node> &GetBone(uint32_t index) const;

private:
	std::shared_ptr<Skeleton> skeleton;
	std::shared_ptr<Renderable> renderable;
};

} // namespace hg
