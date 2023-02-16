// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/collision_pair.h"
#include "engine/mouse_event.h"
#include "engine/render_pass.h"
#include "engine/scene_system.h"
#include "engine/script.h"
#include "foundation/signal.h"
#include <string>

namespace hg {

struct ScriptComponentExecutionContext : ScriptExecutionContext {
	ScriptComponentExecutionContext(std::shared_ptr<Script> s) : script_component(s) {}

	std::string GetDescription() const override;
	ScriptObject GetEnv() const override;

private:
	std::shared_ptr<Script> script_component;
};

class RenderSystemAsync;
class RendererAsync;
class MixerAsync;

/// Global engine environment shared by all scripts in the system.
struct ScriptEngineEnv {
	ScriptEngineEnv(std::shared_ptr<RenderSystemAsync> s, std::shared_ptr<RendererAsync> r, std::shared_ptr<MixerAsync> m) : render_system_async(s), renderer_async(r), mixer_async(m) {}

	std::shared_ptr<RenderSystemAsync> render_system_async;
	std::shared_ptr<RendererAsync> renderer_async;
	std::shared_ptr<MixerAsync> mixer_async;

	float dt{0}; // dt_clock for the current frame (in sec)
};

enum ScriptExecutionMode {
	ScriptExecutionStandalone = 0x01,
	ScriptExecutionEditor = 0x02,
	ScriptExecutionAll = 0xff
};

class VM;

struct Texture;
struct RenderTarget;
struct RenderPrimitiveLists;
struct ScriptRuntimeError;

struct ScriptLogicDlgts {
	std::map<Script *, ScriptObject> update_dlgt, end_draw_dlgt, mouse_event_dlgt, on_collision_dlgt;
};

/// Script system interface
class ScriptSystem : public std::enable_shared_from_this<ScriptSystem>, public SceneSystem {
public:
	explicit ScriptSystem(std::shared_ptr<ScriptEngineEnv> engine_env = nullptr);
	~ScriptSystem();

	const std::string &GetAspect() const override;

	/// Return the script execution mode.
	ScriptExecutionMode GetExecutionMode() const { return execution_mode; }
	/// Set the script execution mode.
	void SetExecutionMode(ScriptExecutionMode mode) { execution_mode = mode; }

	/// Notify a script that a parameter value has been modified.
	void NotifyParameterChange(std::shared_ptr<Script> script, const std::string &name);
	/// Return true if the script is ready (might imply a call to a script function).
	bool TestScriptIsReady(const Script &script);

	/// Get implementation name.
	virtual const std::string &GetImplementationName() const = 0;

	/// Open system.
	virtual bool Open() = 0;
	/// Close system.
	virtual void Close() = 0;

	/// Reset the system, destroy all object references from the system VM.
	virtual void Reset() = 0;

	/// Return the VM for a given script.
	virtual std::shared_ptr<VM> GetVM(const Script &script) = 0;

	typedef Signal<void(const ScriptRuntimeError &)> RuntimeErrorSignal;
	RuntimeErrorSignal runtime_error_signal;

	void ConnectToRenderSystemSignals(RenderSystem &render_system);
	void DisconnectFromRenderSystemSignals(RenderSystem &render_system);

	virtual void SetMouseEvent(MouseEvent event) = 0;

	typedef Signal<void(VM &, const ScriptLogicDlgts &)> UpdateLogicContextSignal;
	UpdateLogicContextSignal update_logic_context_signal;

	/// Emitted right after a VM value has been changed, the VM lock is being held by the caller.
	Signal<void(const std::shared_ptr<Script> &, const std::string &)> script_component_commit_set_signal;

	std::shared_ptr<SceneSystem> shared_from_base() override { return shared_from_this(); }

protected:
	Signal<void(const std::shared_ptr<Script> &, const std::string &)>::Connection script_component_commit_set_connection;

	std::shared_ptr<ScriptEngineEnv> engine_env;
	ScriptExecutionMode execution_mode{ScriptExecutionStandalone};

	struct ParameterChangeNotification {
		ParameterChangeNotification(std::shared_ptr<Script> script, const std::string &name);
		~ParameterChangeNotification();

		std::shared_ptr<Script> script;
		std::string name;
	};

	virtual void commit_NotifyParameterChange(std::shared_ptr<Script> script, const std::string &name) = 0;

	void OnScriptComponentCommitSet(const std::shared_ptr<Script> &script, const std::string &name);

	const std::vector<CollisionPair> *collision_pairs{nullptr};

	virtual void OnRenderSystemBeginDrawFrame(RenderSystem &render_system) {}
	virtual bool OnRenderSystemClearFrame(RenderSystem &render_system) { return false; }
	virtual void OnRenderSystemFrameComplete(RenderSystem &render_system) {}
	virtual void OnRenderSystemEndRenderPass(RenderSystem &render_system, RenderPass::Pass pass) {}

	virtual bool OnRenderSystemApplyPostProcesses(RenderSystem &render_system, const RenderPrimitiveLists &prim_lists, const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &src, const std::shared_ptr<Texture> &dst) { return false; }

	Signal<void(RenderSystem &)>::Connection begin_draw_frame_connection, frame_complete_connection;
	Signal<bool(RenderSystem &)>::Connection clear_frame_connection;
	Signal<void(RenderSystem &, RenderPass::Pass)>::Connection end_render_pass_connection;
	Signal<bool(RenderSystem &, const RenderPrimitiveLists&, const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &src, const std::shared_ptr<Texture> &dst)>::Connection apply_post_processes_connection;
};

} // namespace hg
