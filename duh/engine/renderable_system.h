// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/immediate_renderable.h"
#include "engine/octree_renderable.h"
#include "engine/render_pass.h"
#include "engine/render_system.h"
#include "engine/scene_system.h"
#include "engine/simple_list_renderable.h"

namespace hg {

struct FrameRenderer;
struct DefaultFrameRenderer;

class Environment;

/// Renderable system
class RenderableSystem : public std::enable_shared_from_this<RenderableSystem>, public SceneSystem {
public:
	RenderableSystem(std::shared_ptr<RenderSystem>, bool async = true);

	const std::string &GetAspect() const override;

	void AddComponent(std::shared_ptr<Component> component) override;
	void RemoveComponent(const std::shared_ptr<Component> &component) override;

	void Update(time_ns dt) override;
	void Commit() override;

	/// Can be run from any thread, the render task is added to the update task set.
	void Render(std::shared_ptr<Node> camera);

	/**
		@short Render the complete system for picking.
		@note Must be run from the renderer thread.
	*/
	bool RenderPicking(const std::shared_ptr<RenderTarget> &fbo);

	/// Force rendering of a single render pass shader for debugging purpose.
	void SetDebugRenderPassShader(RenderPass::Shader shader = RenderPass::ShaderCount);

	/// Set the system frame renderer.
	void SetFrameRenderer(std::shared_ptr<FrameRenderer> renderer);

	/// Add a renderable to the system.
	void AddRenderable(std::shared_ptr<Renderable> r) { commit_queue.queue<void>(std::bind(&RenderableSystem::commit_AddRenderable, this, r)); }
	/// Remove a renderable from the system.
	void RemoveRenderable(const std::shared_ptr<Renderable> &r) { commit_queue.queue<void>(std::bind(&RenderableSystem::commit_RemoveRenderable, this, r)); }

	void DrawBuffers(uint32_t idx_count, std::shared_ptr<GpuBuffer> idx, std::shared_ptr<GpuBuffer> vtx, const VertexLayout &vtx_layout, std::shared_ptr<RenderMaterial> mat, const MinMax &minmax, const Matrix4 &world, IndexType idx_type = IndexUShort, GpuPrimitiveType prim_type = GpuPrimitiveTriangle);
	void DrawGeometry(std::shared_ptr<RenderGeometry> geo, const Matrix4 &world);

	void SetEnvironment(std::shared_ptr<Node> camera, std::shared_ptr<LightSystem> light_system, std::shared_ptr<Environment> environment);
	void SetUseCameraView(bool use) { use_camera_view = use; }

	Signal<void(RenderSystem &)> pre_render_frame_signal;
	Signal<void(RenderSystem &)> post_render_frame_signal;

	void DrawDebugPanel(RenderSystem &render_system) override;
	void DrawDebugVisuals(RenderSystem &render_system) override;

	std::shared_ptr<SceneSystem> shared_from_base() override { return shared_from_this(); }

private:
	std::shared_ptr<RenderSystem> render_system;

	std::shared_ptr<SimpleCullingSystem> simple_culling_system;
	std::shared_ptr<OctreeRenderable> octree_culling_system;

	std::shared_ptr<ImmediateRenderable> immediate_renderables[2]; // for immediate draw calls
	uint32_t current_immediate_renderables; // ping-pong between updates

	std::vector<std::shared_ptr<Renderable>> renderables; // the system static root renderable list

	std::shared_ptr<DefaultFrameRenderer> default_frame_renderer;
	std::shared_ptr<FrameRenderer> frame_renderer;

	struct RenderEnvironment {
		std::shared_ptr<Node> camera;
		std::shared_ptr<LightSystem> light_system;
		std::shared_ptr<Environment> environment;
	};

	RenderEnvironment render_env;
	RenderEnvironment current_render_env;

	void UpdateTask(const RenderEnvironment &env);

	bool is_async;
	std::future<void> show_frame_future;

	void Render_async(const RenderEnvironment &env);

	void commit_AddRenderable(const std::shared_ptr<Renderable> &);
	void commit_RemoveRenderable(const std::shared_ptr<Renderable> &);

	void commit_DrawBuffers(uint32_t idx_count, std::shared_ptr<GpuBuffer> idx, std::shared_ptr<GpuBuffer> vtx, const VertexLayout &vtx_layout, std::shared_ptr<RenderMaterial> mat, const MinMax &local_minmax, const Matrix4 &world, IndexType idx_type, GpuPrimitiveType prim_type);
	void commit_DrawGeometry(std::shared_ptr<RenderGeometry> geo, const Matrix4 &world);

	bool enable_immediate_rendering;

	bool dbg_draw_simple_culling_system{false};
	bool dbg_draw_octree_culling_system{false};

	bool use_camera_view{true};
};

/// Frame renderer interface
struct FrameRenderer {
	virtual ~FrameRenderer() {}
	/// Initialize the frame renderer.
	virtual bool Initialize(RenderSystem &render_system) = 0;
	/// Render a frame.
	virtual void RenderFrame(RenderSystem &render_system, LightSystem &light_system, std::vector<std::shared_ptr<Renderable>> &renderables, const std::vector<std::shared_ptr<Component>> &post_processes) = 0;
	/// Shutdown the frame renderer.
	virtual void Shutdown(RenderSystem &render_system) = 0;
	virtual bool Set(const std::string &name, const std::string &value);
	virtual std::string Get(const std::string &name) const;
};

/// Default frame renderer
struct DefaultFrameRenderer : FrameRenderer {
	bool Initialize(RenderSystem &render_system) override { return true; }
	void RenderFrame(RenderSystem &render_system, LightSystem &light_system, std::vector<std::shared_ptr<Renderable>> &renderables, const std::vector<std::shared_ptr<Component>> &post_processes) override;
	void Shutdown(RenderSystem &render_system) override {}

	RenderPass::Shader dbg_render_pass_shader{RenderPass::ShaderCount};
};

} // namespace hg
