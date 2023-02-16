// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/point.h"
#include "foundation/vector2.h"

namespace hg {

/// Rectangle 2D
template <class T>
struct Rect {
	Rect() : sx(0), sy(0), ex(0), ey(0) {}
	Rect(T usx, T usy) : sx(usx), sy(usy), ex(usx), ey(usy) {}
	Rect(T usx, T usy, T uex, T uey) : sx(usx), sy(usy), ex(uex), ey(uey) {}
	Rect(const Rect<T> &b) : sx(b.sx), sy(b.sy), ex(b.ex), ey(b.ey) {}

	inline T GetX() const { return sx; }
	inline T GetY() const { return sy; }
	inline void SetX(T x) {
		auto w = GetWidth();
		sx = x;
		ex = x + w;
	}
	inline void SetY(T y) {
		auto h = GetHeight();
		sy = y;
		ey = y + h;
	}

	inline T GetWidth() const { return ex - sx; }
	inline T GetHeight() const { return ey - sy; }
	void SetWidth(T w) { ex = sx + w; }
	void SetHeight(T h) { ey = sy + h; }

	tVector2<T> GetSize() const { return tVector2<T>(ex - sx, ey - sy); }

	Rect<T> operator*(T v) const { return Rect<T>(sx * v, sy * v, ex * v, ey * v); }
	Rect<T> operator/(T v) const { return Rect<T>(sx / v, sy / v, ex / v, ey / v); }

	bool operator==(const Rect<T> &b) const { return sx == b.sx && sy == b.sy && ex == b.ex && ey == b.ey; }
	bool operator!=(const Rect<T> &b) const { return sx != b.sx || sy != b.sy || ex != b.ex || ey != b.ey; }

	inline bool Inside(T x, T y) const { return x > sx && y > sy && x < ex && y < ey; }

	inline bool FitsInside(const Rect<T> &b) const { return GetWidth() <= b.GetWidth() && GetHeight() <= b.GetHeight(); }
	inline bool Intersects(const Rect<T> &b) const { return !(ex < b.sx || ey < b.sy || sx > b.ex || sy > b.ey); }

	Rect<T> Intersection(const Rect<T> &b) const {
		auto _sx = Max(sx, b.sx), _sy = Max(sy, b.sy),
			 _ex = Min(ex, b.ex), _ey = Min(ey, b.ey);
		auto n_sx = Min(_sx, _ex), n_sy = Min(_sy, _ey),
			 n_ex = Max(_sx, _ex), n_ey = Max(_sy, _ey);
		return Rect<T>(_sx = n_sx, _sy = n_sy, _ex = n_ex, _ey = n_ey);
	}

	Rect<T> Grow(T border) const { return Rect<T>(sx - border, sy - border, ex + border, ey + border); }

	void Set(T usx, T usy, T uex, T uey) {
		sx = usx;
		sy = usy;
		ex = uex;
		ey = uey;
	}
	void Set(T ux = 0, T uy = 0) {
		sx = ux;
		sy = uy;
		ex = ux;
		ey = uy;
	}

	Rect<T> Offset(T x, T y) const { return Rect<T>(sx + x, sy + y, ex + x, ey + y); }
	Rect<T> Cropped(T osx, T osy, T oex, T oey) const { return Rect<T>(sx + osx, sy + osy, ex - oex, ey - oey); }

	static Rect<T> FromWidthHeight(T sx, T sy, T w, T h) { return Rect<T>(sx, sy, sx + w, sy + h); }

	T sx, sy, ex, ey;
};

typedef Rect<int> iRect;
typedef Rect<float> fRect;

template <typename T>
fRect ToFloatRect(const Rect<T> &r) { return fRect(float(r.sx), float(r.sy), float(r.ex), float(r.ey)); }
template <typename T>
iRect ToIntRect(const Rect<T> &r) { return iRect(int(r.sx), int(r.sy), int(r.ex), int(r.ey)); }

template <typename T>
Rect<T> FitRectByWidth(const Rect<T> &src, const Rect<T> &fit_to) {
	Rect<T> out;
	out.sx = src.sx;
	out.ex = src.ex;
	auto dy = (fit_to.GetHeight() * src.GetWidth()) / fit_to.GetWidth() / 2;
	out.sy = (src.sy + src.ey) / 2 - dy;
	out.ey = (src.sy + src.ey) / 2 + dy;
	return out;
}

template <typename T>
Rect<T> FitRectByHeight(const Rect<T> &src, const Rect<T> &fit_to) {
	Rect<T> out;
	out.sy = src.sy;
	out.ey = src.ey;
	auto dx = (fit_to.GetWidth() * src.GetHeight()) / fit_to.GetHeight() / 2;
	out.sx = (src.sx + src.ex) / 2 - dx;
	out.ex = (src.sx + src.ex) / 2 + dx;
	return out;
}

template <typename T>
Rect<T> FitRectByAspectRatio(const Rect<T> &src, const Rect<T> &fit_to) {
	auto dst_ar = float(fit_to.GetWidth()) / fit_to.GetHeight(), src_ar = float(src.GetWidth()) / src.GetHeight();
	return src_ar > dst_ar ? FitRectByHeight(src, fit_to) : FitRectByWidth(src, fit_to);
}

} // namespace hg
