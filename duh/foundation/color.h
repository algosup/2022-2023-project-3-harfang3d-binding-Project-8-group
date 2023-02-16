// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

namespace hg {

/// RGBA floating point color
struct Color {
	static const Color Zero;
	static const Color One;
	static const Color White;
	static const Color Grey;
	static const Color Black;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Orange;
	static const Color Purple;
	static const Color Transparent;

	float r, g, b, a;

	Color() = default;
	Color(float _r, float _g, float _b, float _a = 1) : r(_r), g(_g), b(_b), a(_a) {}

	inline bool operator==(const Color &v) const { return r == v.r && g == v.g && b == v.b && a == v.a; }
	inline bool operator!=(const Color &v) const { return r != v.r || g != v.g || b != v.b || a != v.a; }

	inline Color &operator+=(const Color &c) {
		r += c.r;
		g += c.g;
		b += c.b;
		a += c.a;
		return *this;
	}

	inline Color &operator+=(const float k) {
		r += k;
		g += k;
		b += k;
		a += k;
		return *this;
	}

	inline Color &operator-=(const Color &c) {
		r -= c.r;
		g -= c.g;
		b -= c.b;
		a -= c.a;
		return *this;
	}

	inline Color &operator-=(const float k) {
		r -= k;
		g -= k;
		b -= k;
		a -= k;
		return *this;
	}

	inline Color &operator*=(const Color &c) {
		r *= c.r;
		g *= c.g;
		b *= c.b;
		a *= c.a;
		return *this;
	}

	inline Color &operator*=(const float k) {
		r *= k;
		g *= k;
		b *= k;
		a *= k;
		return *this;
	}

	inline Color &operator/=(const Color &c) {
		r /= c.r;
		g /= c.g;
		b /= c.b;
		a /= c.a;
		return *this;
	}

	inline Color &operator/=(const float k) {
		float i = 1.0f / k;
		r *= i;
		g *= i;
		b *= i;
		a *= i;
		return *this;
	}

	inline float operator[](int n) const { return (&r)[n]; }
	inline float &operator[](int n) { return (&r)[n]; }
};

} // namespace hg
