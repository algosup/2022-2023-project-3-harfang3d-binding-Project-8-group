// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/base_type_reflection.h"
#include "foundation/foundation_reflection.h"
#include "foundation/quaternion.h"
#include "foundation/reflection.h"
#include "foundation/time.h"
#include "foundation/vector3_api.h"
#include <vector>
#include <array>

namespace hg {

template <typename T>
struct AnimKey {
	AnimKey(time_ns t_, const T &v_) : t(t_), v(v_) {}
	time_ns t;
	T v;
};

//
class AnimTrack {
public:
	AnimTrack() = default;
	AnimTrack(std::string target_) : target(std::move(target_)) {}
	virtual ~AnimTrack() = default;

	std::string target;

	virtual std::string GetType() const = 0;
	virtual Type *GetStorageType() const = 0;

	virtual bool RemoveKey(time_ns t) = 0;
	virtual bool RemoveKey(size_t i) = 0;
	virtual void Clear() = 0;

	virtual time_ns GetDuration() const = 0;
	virtual size_t GetKeyCount() const = 0;
	virtual bool GetRange(time_ns &t_start, time_ns &t_end) const = 0;
	virtual bool GetValue(time_ns t, void *o) const = 0;

	bool Evaluate(time_ns t, void *v) const { return this->GetValue(t, v); }
};

template <typename T>
class AnimTrackT : public AnimTrack {
public:
	AnimTrackT() = default;
	AnimTrackT(std::string target) : AnimTrack(target) {}

	std::string GetType() const override { return "AnimTrackT"; }
	Type *GetStorageType() const override { return g_type_registry.get().GetType<T>(); }

	void SetKey(time_ns t, const T &v) {
		auto begin = std::begin(keys), end = std::end(keys);

		auto i = end;

		if (i != begin)
			forever {
				--i;
				if (i->t == t)
					break;

				if (i->t < t) {
					++i;
					break;
				}

				if (i == begin)
					break;
			}

		if ((i != end) && (i->t == t))
			i->v = v;
		else
			keys.emplace(i, t, v);
	}

	bool RemoveKey(time_ns t) override {
		for (auto i = std::begin(keys); i < std::end(keys); ++i)
			if (i->t == t) {
				keys.erase(i);
				return true;
			}
		return false;
	}

	bool RemoveKey(size_t i) override {
		if (i >= keys.size())
			return false;
		keys.erase(std::begin(keys) + i);
		return true;
	}

	void Clear() override { keys.clear(); }

	enum KfsAt { None,
		First,
		Mid,
		Last };

	KfsAt GetKeysAt(time_ns t, std::array<const AnimKey<T> *, 2> &out_kfs) const {
		auto begin = std::begin(keys), end = std::end(keys);

		if (begin == end)
			return None;

		auto i = end;

		forever {
			--i;
			if (i->t < t)
				break;
			if (i == begin) {
				out_kfs[0] = out_kfs[1] = &(*i); // t < t_min
				return First;
			}
		}

		if (i == end || (i + 1) == end) {
			out_kfs[0] = out_kfs[1] = &(*(i - 1)); // t > t_max
			return Last;
		}

		out_kfs[0] = &(*i);
		out_kfs[1] = &(*(i + 1));
		return Mid;
	}

	time_ns GetDuration() const override {
		const auto size = keys.size();
		return size > 0 ? keys[size - 1].t - keys[0].t : 0;
	}

	size_t GetKeyCount() const override { return keys.size(); }

	bool GetRange(time_ns &t_start, time_ns &t_end) const override {
		const auto size = keys.size();
		if (size == 0)
			return false;

		t_start = keys[0].t;
		t_end = keys[size - 1].t;
		return true;
	}

	bool GetValue(time_ns t, void *v) const override {
		std::array<const AnimKey<T> *, 2> _kfs;
		auto pos = this->GetKeysAt(t, _kfs);

		switch (pos) {
			case None:
				return false;

			case First:
				*(T *)v = _kfs[0]->v;
				return true;
			case Last:
				*(T *)v = _kfs[_kfs.size() - 1]->v;
				return true;

			default:
				break;
		}

		float dt = time_to_sec_f(_kfs[1]->t - _kfs[0]->t);

		if (dt) {
			float k = time_to_sec_f(t - _kfs[0]->t) / dt;
			*(T *)v = _kfs[0]->v + (_kfs[1]->v - _kfs[0]->v) * k;
		} else {
			*(T *)v = _kfs[0]->v;
		}
		return true;
	}

	bool GetValue(time_ns t, T &v) const { return this->GetValue(t, &v); }

	std::vector<AnimKey<T>> &GetKeys() { return keys; }
	const std::vector<AnimKey<T>> &GetKeys() const { return keys; }

private:
	std::vector<AnimKey<T>> keys;
};

class AnimTrackRotation : public AnimTrackT<Vector3> {
public:
	AnimTrackRotation() = default;
	AnimTrackRotation(std::string target) : AnimTrackT<Vector3>(target) {}

	std::string GetType() const override { return "AnimTrackRotation"; }

	bool GetValue(time_ns t, void *o) const override {
		std::array<const AnimKey<Vector3> *, 2> _kfs;
		if (this->GetKeysAt(t, _kfs) == None)
			return false;

		float dt = time_to_sec_f(_kfs[1]->t - _kfs[0]->t);

		if (dt) {
			float k = time_to_sec_f(t - _kfs[0]->t) / dt;
			*(Vector3 *)o = Quaternion::Slerp(k, Quaternion::FromEuler(_kfs[0]->v), Quaternion::FromEuler(_kfs[1]->v)).ToEuler();
		} else {
			*(Vector3 *)o = _kfs[0]->v;
		}
		return true;
	}
};

//
template <typename T> bool TestAnimTrackValueForEquality(const T &v_a, const T &v_b, float /*epsilon*/) { return v_a == v_b; }

bool TestAnimTrackValueForEquality(const Vector3 &v_a, const Vector3 &v_b, float epsilon);
bool TestAnimTrackValueForEquality(const Quaternion &v_a, const Quaternion &v_b, float epsilon);

//
template <typename T, typename O = T>
size_t CleanAnimTrackT(AnimTrackT<T> &track, float epsilon = 0.0000001f) {
	std::vector<AnimKey<T>> &keys = track.GetKeys();
	size_t count = keys.size(), remove_count = 0;

	while (true) {
		auto old_count = count;

		if (count == 2) {
			if (TestAnimTrackValueForEquality(keys[0].v, keys[1].v, epsilon)) {
				keys.erase(std::begin(keys));
				--count;
			}
		} else {
			for (size_t i = 1; i < (count - 1);) {
				if (TestAnimTrackValueForEquality(keys[i].v, keys[i + 1].v, epsilon) && TestAnimTrackValueForEquality(keys[i].v, keys[i - 1].v, epsilon)) {
					keys.erase(std::begin(keys) + i);
					--count;
				} else {
					++i;
				}
			}
		}

		auto pass_remove = old_count - count;
		if (!pass_remove)
			break;

		remove_count += pass_remove;
	}

	return remove_count;
}

/*!
	@short Animation
	Holds a group of animation tracks together.
*/
struct Anim {
	Anim() = default;
	Anim(std::string target_) : target(std::move(target_)) {}

	std::string target;
	std::vector<std::shared_ptr<AnimTrack>> tracks;

	bool GetTracksRange(time_ns &t_start, time_ns &t_end) const;
};

/*!
	@short Animation take
	Holds a group of animation together.
*/
struct AnimTake {
	AnimTake() = default;
	AnimTake(std::string name_) : name(std::move(name_)) {}

	std::string name;
	time_ns start{0}, end{0};

	std::vector<std::shared_ptr<Anim>> anims;

	bool is_instantiated{false};

	bool GetAnimsRange(time_ns &t_start, time_ns &t_end) const;
};

//
struct BoundAnimTrack {
	virtual ~BoundAnimTrack() = default;

	std::shared_ptr<AnimTrack> track;

	virtual void Apply(time_ns t) const = 0;
};

//
struct BoundAnim {
	std::vector<std::unique_ptr<BoundAnimTrack>> tracks;

	void Apply(time_ns t) const;
};

bool GetBoundAnimRange(const BoundAnim &bound_anim, time_ns &t_range_start, time_ns &t_range_end);

//
enum AnimLoop {
	AnimOnce,
	AnimRepeat,
	AnimPingPong
};

time_ns TimeRepeat(time_ns t, time_ns start, time_ns end);
time_ns TimePingPong(time_ns t, time_ns start, time_ns end);

} // namespace hg
