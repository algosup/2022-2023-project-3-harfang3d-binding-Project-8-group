// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/script_object.h"
#include "foundation/reflection.h"
#include "foundation/signal.h"
#include <memory>
#include <string>

namespace hg {

struct ScriptObject;
struct ScriptRuntimeError;
class ScriptExecutionContext;

class VM : public std::enable_shared_from_this<VM> {
public:
	VM(const std::string &_name) : name(_name) {}
	virtual ~VM() {}

	const std::string &GetName() const { return name; }
	void SetName(std::string _name) { name = std::move(_name); }

	bool TryLock() { return vm_mutex.try_lock(); }
	void Lock() { vm_mutex.lock(); }
	void Unlock() { vm_mutex.unlock(); }

	/// Compile a script to a chunk.
	virtual bool Compile(const std::string &source, const std::string &name, ScriptObject &chunk_out) = 0;
	/// Execute a source script.
	virtual bool Execute(const std::string &source, const std::string &name, ScriptObject *env = nullptr, std::vector<ScriptObject> *ret_vals = nullptr, std::shared_ptr<ScriptExecutionContext> exec_ctx = nullptr) = 0;

	virtual bool Open() = 0;
	virtual void Close() = 0;
	virtual bool IsOpen() const = 0;

	virtual const char *GetInterpreter() const { return "Undefined"; }

	virtual ScriptObject CreateNull() = 0;
	virtual ScriptObject CreateEnvironment(bool transfer_safe_global_symbols = false) = 0;
	virtual ScriptObject CreateTable() = 0;
	virtual ScriptObject CreateArray() = 0;

	/// Retrieve a VM object.
	virtual TypeValue ObjectToValue(const ScriptObject &object, const std::string &cast_to = std::string()) = 0;
	virtual ScriptObject ValueToObject(const TypeValue &val) = 0;

	inline ScriptObject CreateObject(const TypeValue &val) { return ValueToObject(val); }

	virtual ScriptObject Get(const std::string &key) = 0;
	virtual bool Set(const std::string &key, const ScriptObject &val) = 0;
	virtual ScriptObject Get(const std::string &key, const ScriptObject &context) = 0;
	virtual bool Set(const std::string &key, const ScriptObject &val, const ScriptObject &context) = 0;

	virtual ScriptObject Get(int index, const ScriptObject &array_object) = 0;
	virtual bool Set(int index, const ScriptObject &val, const ScriptObject &array_object) = 0;

	TypeValue GetValue(const std::string &key);
	bool SetValue(const std::string &key, const TypeValue &val);
	TypeValue GetValue(const std::string &key, const ScriptObject &context);
	bool SetValue(const std::string &key, const TypeValue &val, const ScriptObject &context);

	TypeValue GetValue(int index, const ScriptObject &array_object);
	bool SetValue(int index, const TypeValue &val, const ScriptObject &array_object);

	void PushExecutionContext(std::shared_ptr<ScriptExecutionContext> ctx) { exec_ctx_stack.emplace_back(std::move(ctx)); }
	void PopExecutionContext() { exec_ctx_stack.pop_back(); }

	std::shared_ptr<ScriptExecutionContext> GetCurrentExecutionContext() const { return exec_ctx_stack.size() > 0 ? exec_ctx_stack.back() : nullptr; }

	/// Call into the VM.
	virtual bool Call(const ScriptObject &function, const std::vector<ScriptObject> &parms = {}, std::vector<ScriptObject> *ret_vals = nullptr, std::shared_ptr<ScriptExecutionContext> exec_ctx = nullptr) = 0;

	Signal<void(const ScriptRuntimeError &)> runtime_error_signal;
	bool emit_runtime_error_signal{true};

protected:
	std::mutex vm_mutex;
	std::vector<std::shared_ptr<ScriptExecutionContext>> exec_ctx_stack;

	std::string name;
};

extern Signal<void(VM *)> vm_open_signal, vm_close_signal;

//
class VMLock {
public:
	VM &GetVM() const { return vm; }

	explicit VMLock(VM &v) : vm(v) { vm.Lock(); }
	~VMLock() { vm.Unlock(); }

private:
	VM &vm;
};

} // namespace hg
