// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/script_system.h"

namespace hg {

class LuaVM;

class LuaSystem : public ScriptSystem {
public:
	typedef uint8_t VMIndex; // max 256 VM context type

	explicit LuaSystem(std::shared_ptr<ScriptEngineEnv> env = nullptr);
	~LuaSystem();

	const std::string &GetImplementationName() const override;

	void AddComponent(std::shared_ptr<Component> component) override;
	void RemoveComponent(const std::shared_ptr<Component> &component) override;

	void Update(time_ns dt) override;
	void Commit() override;

	bool Open() override;
	void Close() override;
	void Reset() override;

	bool IsOpen() const;
	void OnRuntimeError(const ScriptRuntimeError &error);

	std::shared_ptr<VM> GetVM(const Script &component) override;

	void SetMouseEvent(MouseEvent event) override { mouse_event = event; };

	void DrawDebugPanel(RenderSystem &render_system) override;

	struct InitPending {
		enum Type : uint8_t {
			Uninit,
			Init
		};

		InitPending() = default;
		InitPending(std::shared_ptr<Script> s_, Type i_, VMIndex v_) : s(std::move(s_)), init(i_), index(v_) {}

		std::shared_ptr<Script> s;

		Type init{Init};
		VMIndex index{0};
	};

	struct Context {
		Context();
		~Context();

		std::shared_ptr<LuaVM> vm;

		std::vector<ParameterChangeNotification> queued_parameter_change_notifications;

		bool Open();
		void Close();

		std::vector<InitPending> init_pending;

		bool debug{false};
	};

private:
	struct ContextDesc { // 2 bytes
		enum Type : uint8_t {
			Logic,
			Render
		};

		ContextDesc() = default;
		ContextDesc(Type t, uint8_t i) : type(t), index(i) {}

		Type type{Logic};
		VMIndex index{0}; // VM index
	};

	std::map<const Script *, ContextDesc> script_context_map;

	bool InitializeScriptCommon(Context &ctx, const std::shared_ptr<Script> &s, ContextDesc desc);
	bool UninitializeScriptCommon(Context &ctx, const std::shared_ptr<Script> &s);

	void ProcessParameterChangeNotifications(Context &ctx);

	std::vector<InitPending> logic_init_pending;

	//
	struct Statistics {
		time_ns update{0}, garbage_collect{0};
	};

	uint8_t logic_ctx_assign;
	std::vector<Context> logic_ctx;
	std::vector<ScriptLogicDlgts> logic_dlgt;
	std::vector<Statistics> logic_stat;

	ScriptObject S_; // table shared by all components running on the master logic VM

	void ProcessLogicPendingInit(std::vector<InitPending> &init);
	void UpdateLogicContext(uint32_t idx);

	void RemoveScriptDelegates(Context &ctx, Script &script_component);

	//
	MouseEvent mouse_event;

	//
	Context render_ctx; // rendering context
	Statistics render_stat;

	void commit_NotifyParameterChange(std::shared_ptr<Script> script, const std::string &name) override;

	//
	void ProcessLogicInit();
	void RunGarbageCollection(Context &ctx, Statistics &stats);

	//
	void ProcessRenderContextInit(LuaSystem::Context &ctx);

	void OnRenderSystemBeginDrawFrame(RenderSystem &render_system) override;
	bool OnRenderSystemClearFrame(RenderSystem &render_system) override;
	void OnRenderSystemFrameComplete(RenderSystem &render_system) override;
	void OnRenderSystemEndRenderPass(RenderSystem &render_system, RenderPass::Pass pass) override;

	bool OnRenderSystemApplyPostProcesses(RenderSystem &render_system, const RenderPrimitiveLists &prim_lists, const std::shared_ptr<RenderTarget> &rt, const std::shared_ptr<Texture> &src, const std::shared_ptr<Texture> &dst) override;

	std::map<Script *, ScriptObject> begin_draw_frame_dlgt, clear_frame_dlgt, end_draw_dlgt, end_render_pass_dlgt, post_process_dlgt;

	void AddRenderDelegates(LuaVM &vm, Script &s);
	void RemoveRenderDelegates(Script &s);

	//
	inline Context *GetContext(ContextDesc desc) { return desc.type == ContextDesc::Render ? &render_ctx : &logic_ctx[desc.index]; }
	Context *GetContext(const VM &vm);

	//
	void DrawContextDebugPanel(const std::string &name, LuaSystem::Context &ctx);
};

} // namespace hg
