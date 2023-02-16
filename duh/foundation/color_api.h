// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/color.h"
#include "foundation/math.h"
#include "foundation/math.h"

namespace hg {

inline Color operator+(const Color &a, const Color &b) { return Color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a); }
inline Color operator+(const Color &a, const float v) { return Color(a.r + v, a.g + v, a.b + v, a.a + v); }
inline Color operator-(const Color &a, const Color &b) { return Color(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a); }
inline Color operator-(const Color &a, const float v) { return Color(a.r - v, a.g - v, a.b - v, a.a - v); }
inline Color operator*(const Color &a, const Color &b) { return Color(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a); }
inline Color operator*(const Color &a, const float v) { return Color(a.r * v, a.g * v, a.b * v, a.a * v); }
inline Color operator/(const Color &a, const Color &b) { return Color(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a); }
inline Color operator/(const Color &a, const float v) { return Color(a.r / v, a.g / v, a.b / v, a.a / v); }

/**
	@short Return a grayscale value representing this color
	The grayscale value is computed accounting for the human eye color intensity perception.
*/
inline float ColorToGrayscale(const Color &c) { return 0.3f * c.r + 0.59f * c.g + 0.11f * c.b; }

/// Return the color object as an RGBA value.
uint32_t ColorToRGBA32(const Color &c);
uint32_t ColorToABGR32(const Color &c);
/// Load the color object from an RGBA value.
Color ColorFromRGBA32(uint32_t rgba32);
Color ColorFromABGR32(uint32_t abgr32);

/// Convert from ARGB to RGBA.
inline uint32_t ARGB32ToRGBA32(uint32_t argb) { return ((argb & 0xff) << 24) + (((argb >> 8) & 0xff) << 16) + (((argb >> 16) & 0xff) << 8) + ((argb >> 24) & 0xff); }

/// Vector squared distance.
inline float Dist2(const Color &i, const Color &j) { return (j.r - i.r) * (j.r - i.r) + (j.g - i.g) * (j.g - i.g) + (j.b - i.b) * (j.b - i.b) + (j.a - i.a) * (j.a - i.a); }
/// Vector distance.
inline float Dist(const Color &i, const Color &j) { return Sqrt(Dist2(i, j)); }

/// Compare two colors with a configurable threshold.
inline bool AlmostEqual(const Color &a, const Color &b, float epsilon) { return Abs(a.r - b.r) <= epsilon && Abs(a.g - b.g) <= epsilon && Abs(a.b - b.b) <= epsilon && Abs(a.a - b.a) <= epsilon; }

/// Scale the chroma component of a color, return the result as a new color.
inline Color ChromaScale(const Color &c, float k) { return Color(c.r * k, c.g * k, c.b * k, c.a); }
/// Scale the alpha component of a color, return the result as a new color.
inline Color AlphaScale(const Color &c, float k) { return Color(c.r, c.g, c.b, c.a * k); }

/// Clamp color components to [min;max].
Color Clamp(const Color &c, float min, float max);
/// Clamp color components to [min;max].
Color Clamp(const Color &c, const Color &min, const Color &max);
/// Clamp color magnitude to [min;max].
Color ClampMagnitude(const Color &c, float min, float max);

struct Vector3;
Color ColorFromVector3(const Vector3 &);

struct Vector4;
Color ColorFromVector4(const Vector4 &);

} // namespace hg
