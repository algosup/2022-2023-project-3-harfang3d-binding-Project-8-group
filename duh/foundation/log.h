// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/format.h" // this should be the only place format is included in a header
#include "foundation/global_object.h"
#include "foundation/log_tail.h"
#include "foundation/signal.h"
#include "foundation/timer.h"
#include <mutex>
#include <vector>

namespace hg {

enum LogLevel {
	LogMessage = 0x1,
	LogWarning = 0x2,
	LogError = 0x4,
	LogDebug = 0x8,
	LogAll = 0xff
};

struct LogEntry {
	LogEntry(std::string m, std::string p, std::string d) : msg(std::move(m)), prefix(std::move(p)), details(std::move(d)) {}
	std::string msg, prefix, details;
};

using LogQueue = std::vector<LogEntry>;

class LogSystem {
public:
	LogSystem();

	/// Immediate log output.
	void (*immediate_log_output)(const std::string &timestamp, const std::string &msg, const std::string &prefix, const std::string &details, LogLevel output_mask, bool output_details);

	/// This signal is periodically triggered from the timer thread with new log entries.
	Signal<void(const LogQueue &)> on_log_signal;

	/// Return the log tail object.
	const LogTail &GetTail() const { return tail; }

	void Output(const std::string &msg, const std::string &prefix, const std::string &details);
	void Flush();

	void Shutdown();

private:
	void ProcessLogQueue();

	std::mutex queue_mutex;
	LogQueue queue;

	std::mutex tail_mutex;
	LogTail tail;

	friend void SetLogLevel(LogLevel level);
	LogLevel default_log_output_level_mask{LogLevel(LogWarning | LogError)};

	friend void SetLogIsDetailed(bool);
	bool default_log_output_is_detailed{false};

	timer_thread::handle timer_handle{timer_thread::null_handle};
};

extern global_object<LogSystem> g_log;

/// Set the default log level mask.
void SetLogLevel(LogLevel level);
/// Enable default log detailed output.
void SetLogIsDetailed(bool is_detailed);

/// Immediately flush queued log output.
void FlushLog();

void log(const std::string &msg, const std::string &prefix = std::string(), const std::string &details = std::string());
void warn(const std::string &msg, const std::string &details = std::string());
void error(const std::string &msg, const std::string &details = std::string());
void debug(const std::string &msg, const std::string &details = std::string());

} // namespace hg
