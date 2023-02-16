// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/component.h"
#include "engine/vm.h"
#include "foundation/mixed_block_allocator.h"
#include "foundation/reflection.h"
#include "foundation/time.h"
#include <map>
#include <memory>

struct lua_State;
struct lua_Debug;

namespace hg {

class LuaObjectImpl : public ScriptObject::Impl {
public:
	explicit LuaObjectImpl(lua_State *L_);
	LuaObjectImpl(lua_State *L_, int _ref) : L(L_), ref(_ref) {}
	~LuaObjectImpl();

	void Clear();

	int Get() const { return ref; }
	void Set(int ref);

	bool IsValid() const override;
	bool IsNull() const override;

private:
	lua_State *L;
	int ref;
};

struct ScriptCallstack;

/// Lua VM
class LuaVM : public VM {
public:
	typedef MixedBlockAllocator<4> Allocator;

	LuaVM(const std::string &name) : VM(name) {}

	bool Open() override;
	void Close() override;
	bool IsOpen() const override;

	const char *GetInterpreter() const override;

	void InstallExecutionWatchdog(time_ns timeout);

	lua_State *GetLuaState() const { return L; }

	/// Push an object on the Lua stack.
	bool Push(const ScriptObject &object);
	/// Pop an object from the Lua stack.
	ScriptObject Pop();

	/// Compile a script to a chunk.
	bool Compile(const std::string &source, const std::string &name, ScriptObject &chunk_out) override;
	/// Execute a script.
	bool Execute(const std::string &source, const std::string &name, ScriptObject *env = nullptr, std::vector<ScriptObject> *ret_vals = nullptr, std::shared_ptr<ScriptExecutionContext> exec_ctx = nullptr) override;

	ScriptObject CreateNull() override;
	ScriptObject CreateEnvironment(bool transfer_safe_global_symbols = false) override;
	ScriptObject CreateTable() override;
	ScriptObject CreateArray() override;

	/// Retrieve a VM object.
	ScriptObject Get(const std::string &key) override;
	bool Set(const std::string &key, const ScriptObject &val) override;
	ScriptObject Get(const std::string &key, const ScriptObject &context) override;
	bool Set(const std::string &key, const ScriptObject &val, const ScriptObject &context) override;

	ScriptObject Get(int index, const ScriptObject &array_object) override;
	bool Set(int index, const ScriptObject &val, const ScriptObject &array_object) override;

	TypeValue ObjectToValue(const ScriptObject &, const std::string &cast_to = std::string()) override;
	ScriptObject ValueToObject(const TypeValue &) override;

	template <class T>
	T CastObject(const ScriptObject &o, T default_value, const std::string &cast_to = std::string()) {
		auto v = ObjectToValue(o, cast_to);
		return v.GetType() == g_type_registry.get().GetType<T>() ? v.Cast<T>() : default_value;
	}

	bool Call(const ScriptObject &func, const std::vector<ScriptObject> &parms = {}, std::vector<ScriptObject> *ret_vals = nullptr, std::shared_ptr<ScriptExecutionContext> exec_ctx = nullptr) override;

	void ResetExecutionWatchdog();

	/// Capture current execution callstack.
	void CaptureCallstack(ScriptCallstack &callstack);

private:
	friend void lua_watchdog_hook(lua_State *L, lua_Debug *ar);
	friend int lua_error_handler(lua_State *L);

	lua_State *L{nullptr};
	Allocator allocator;

	bool GatherReturnValues(lua_State *L, int start_index, std::vector<ScriptObject> &ret_vals);

	time_ns exec_watchdog_last_update{0}, exec_watchdog_timeout{time_from_sec(1)};
};

std::shared_ptr<LuaVM> LuaVMFromLuaState(lua_State *L);

void InstallCustomPackagePath(lua_State *L);
void InstallCustomPackageLoader(lua_State *L);

} // namespace hg
