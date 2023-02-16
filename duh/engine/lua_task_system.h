// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#include "foundation/task_system.h"
#include "foundation/reflection.h"

namespace hg {

class LuaVM;
struct ScriptObject;

class LuaTask {
	friend class LuaTaskSystem;

protected:
	std::vector<ScriptObject> chunks; // the task chunk compiled on each system available logic VM
};

enum LuaTaskState {
	LuaTaskPending,
	LuaTaskRunning,
	LuaTaskComplete,
	LuaTaskFailed
};

class LuaTaskHandle {
	friend class LuaTaskSystem;

public:
	/// Return the task state.
	LuaTaskState GetState() const { return state; }
	/// Return true if the task is complete or failed.
	bool IsCompleteOrFailed() const { return state > 1; }

	/// Return an array of results from the task.
	const std::vector<TypeValue> &GetResults() const { return results; }

protected:
	std::atomic<LuaTaskState> state{LuaTaskPending};
	std::vector<TypeValue> results;
};

class LuaTaskSystem {
public:
	bool Start(size_t worker_count = 0);
	void Stop();

	std::shared_ptr<LuaTask> PrepareTask(const std::string &source, const std::string &name);
	std::shared_ptr<LuaTaskHandle> RunTask(std::shared_ptr<LuaTask> task, const std::vector<TypeValue> &args);

	size_t GetWorkerCount() const { return lua_vm.size(); }

private:
	void RunTaskParallel(std::shared_ptr<LuaTask> task, std::shared_ptr<LuaTaskHandle> handle, const std::vector<TypeValue> &args);

	std::vector<std::shared_ptr<LuaVM>> lua_vm;
	task_system lua_task_system;
};

} // namespace hg
