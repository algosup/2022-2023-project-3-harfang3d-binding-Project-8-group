// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "engine/script_execution_context.h"
#include "engine/script_object.h"
#include "foundation/reflection.h"

#undef Expose

namespace hg {

// A script parameter editable in the editor property panel.
struct ScriptParam {
	ScriptParam() = default;
	ScriptParam(std::string name_, std::string type_) : name(name_), type(type_) {}

	std::string name, type;
	TypeValue value;
};

struct ScriptParamList : std::vector<std::shared_ptr<ScriptParam>> {}; // type used to specialize serialization code

// Used to store a (non parameter) value set before the component is setup.
struct ScriptSetValue {
	ScriptSetValue() = default;
	ScriptSetValue(std::string _name, TypeValue _value) : name(std::move(_name)), value(std::move(_value)) {}

	std::string name;
	TypeValue value;
};

/// Script component interface
class Script : public Component {
public:
	Script(const std::string &path = std::string());

	comp_decl_member_get_sync_set_async_noimpl(Script, std::string, Path, path)

	inline bool IsInitialized() const { return is_initialized; }

	ScriptParamList parameters;
	std::shared_ptr<ScriptParam> GetParam(const std::string &name) const;

	ComponentState GetState() const override;

	/// Directly retrieve a value from the component environment with a blocking call.
	TypeValue BlockingGet(const std::string &name) const;

	/// Get a value from the component exposed values.
	TypeValue Get(const std::string &name) const;
	/// Set a value in the component environment.
	void Set(const std::string &name, const TypeValue &value);

	/// Expose a named value.
	void Expose(const std::string &name, const TypeValue &value);

	void Call(const std::string &name, const std::vector<TypeValue> &parms);

	std::vector<ScriptSetValue> set_values;

	virtual ScriptObject GetEnv() { return env_public; }

	ScriptObject env;
	ScriptObject env_public; // execution env as returned by GetEnv

	ScriptObject is_ready_fn; // IsReady script delegate

	std::shared_ptr<ScriptExecutionContext> exec_ctx;

private:
	std::map<std::string, TypeValue> exposed_values;

	friend void ScriptSetIsInitialized(Script &script, bool is_initialized);
	friend void ScriptSetInternalComponentState(Script &script, ComponentState state);
	friend ComponentState ScriptGetInternalComponentState(const Script &script);

	bool is_initialized;
	ComponentState state;

	void commit_Set(const std::string &name, const TypeValue &value);
	void commit_Expose(const std::string &name, const TypeValue &value);
	void commit_Call(const std::string &name, const std::vector<TypeValue> &parms);
};

/// Logic script component interface
struct LogicScript : public std::enable_shared_from_this<LogicScript>, public Script {
	decl_component_defaults(LogicScript, "LogicScript", "Script");

	LogicScript(const std::string &path = std::string());

	comp_decl_member_get_sync_set_async_noimpl(LogicScript, bool, RunOnMasterVM, run_on_master_vm);

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }
};

/// Render script component interface
struct RenderScript : public std::enable_shared_from_this<RenderScript>, public Script {
	decl_component_defaults(RenderScript, "RenderScript", "Script");

	RenderScript(const std::string &path = std::string());

	std::shared_ptr<Component> shared_from_base() override { return shared_from_this(); }

	ScriptObject begin_draw_frame, end_draw_frame;
};

} // namespace hg
