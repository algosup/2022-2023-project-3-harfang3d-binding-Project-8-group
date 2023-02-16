// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/call.h"
#include "foundation/global_object.h"
#include "foundation/signal.h"
#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <string>

namespace hg {

enum GpuResourceState {
	GpuResourceNotReady,
	GpuResourceReady,
	GpuResourceFailed
};

/// Base resource
struct GpuResource : std::enable_shared_from_this<GpuResource> {
	explicit GpuResource(std::string name = "Resource") : name(std::move(name)) {}
	GpuResource(const GpuResource &) = delete;
	virtual ~GpuResource();

	const std::string &GetName() const { return name; }
	void SetName(const std::string &_name) { name = _name; }

	virtual bool IsReadyOrFailed() const;
	virtual bool IsFailed() const;
	virtual bool IsReady() const;

	void SetReady() const;
	void SetFailed() const;
	void SetNotReady() const;

private:
	mutable std::atomic<GpuResourceState> resource_state{GpuResourceNotReady};
	std::string name;
};

struct GpuResourceStateChangeCallbacks {
	std::mutex mutex;
	std::map<const GpuResource *, std::vector<std::function<void()>>> callbacks;
};

extern global_object<GpuResourceStateChangeCallbacks> g_gpu_resource_state_change_callbacks;

void QueueOnGpuResourceReadyOrFailed(const GpuResource *res, std::function<void()> fn);

} // namespace hg
