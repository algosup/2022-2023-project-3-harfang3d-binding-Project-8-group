// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/global_object.h"
#include "foundation/time.h"
#include <mutex>
#include <thread>
#include <vector>

namespace hg {

struct ProfilerFrame {
	uint32_t frame;

	struct Section {
		Section() = default;
		Section(const Section &) = default;
		Section(Section &&s) : thread_id(s.thread_id), start(s.start), end(s.end), details(std::move(s.details)) {}

		std::thread::id thread_id;
		time_ns start{0}, end{0};
		std::string details;
	};

	struct Task {
		std::string name;
		time_ns duration;
		std::vector<size_t> section_indexes;
	};

	std::vector<Section> sections;
	std::vector<Task> tasks;

	time_ns start, end;
};

class Profiler {
public:
	typedef size_t SectionIndex;

	void EndFrame();

	uint32_t GetFrame() const { return frame; }
	const ProfilerFrame &GetLastFrameProfile() const { return last_frame_profile; }

	/// Begin a performance section.
	SectionIndex BeginSection(const std::string &name, const std::string &section_details = std::string());
	/// End a performance section.
	void EndSection(SectionIndex section_index);

	void Lock();
	void Unlock();

private:
	std::mutex lock;

	size_t GetTaskBucket(const std::string &name) const;
	size_t GetTaskInBucket(size_t bucket_index, const std::string &name);

	std::vector<size_t> task_bucket[256];

	struct Task {
		std::string name;
		std::vector<size_t> section_indexes;
	};

	std::vector<Task> tasks;

	struct Section {
		std::thread::id thread_id;
		time_ns start{0}, end{0};
		std::string details;
	};

	std::vector<Section> sections;

	void CaptureFrame(ProfilerFrame &frame);

	uint32_t frame{0};
	ProfilerFrame last_frame_profile;
};

class ProfilerPerfSection {
public:
	ProfilerPerfSection(const std::string &task_name, const std::string &section_details = std::string());
	~ProfilerPerfSection();

	void EndSection();

private:
	bool open;
	Profiler::SectionIndex section_index;
};

extern global_object<Profiler> g_profiler;

} // namespace hg
