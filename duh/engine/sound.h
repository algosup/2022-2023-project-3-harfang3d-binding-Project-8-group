// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <atomic>
#include <string>

namespace hg {

/// Mixer sound object
struct Sound {
	Sound() = default;
	explicit Sound(const std::string &_name) : name(_name) {}
	virtual ~Sound() = default;

	const std::string &GetName() const { return name; }

	virtual bool IsReady() const { return is_ready != 0; }

	void SetReady() { is_ready = 1; }
	void SetNotReady() { is_ready = 0; }

private:
	std::atomic<bool> is_ready;
	std::string name;
};

} // namespace hg
