// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/font_engine.h"
#include "foundation/matrix3.h"
#include "foundation/vector.h"
#include "foundation/vector4.h"
#include <memory>

#undef DrawText

namespace hg {

enum PictureFormat {
	PictureGray8,
	PictureGray16,
	PictureGrayF,
	PictureRGB555,
	PictureRGB565,
	PictureRGB8,
	PictureBGR8,
	PictureRGBA8,
	PictureBGRA8,
	PictureARGB8,
	PictureABGR8,
	PictureRGB16,
	PictureBGR16,
	PictureRGBA16,
	PictureBGRA16,
	PictureARGB16,
	PictureABGR16,
	PictureRGBF,
	PictureBGRF,
	PictureRGBAF,
	PictureBGRAF,
	PictureARGBF,
	PictureABGRF,
	PictureInvalidFormat
};

enum BrushMode {
	BrushNone,
	BrushSolid
};

enum PenMode {
	PenNone,
	PenSolid
};

enum PenCap {
	ButtCap,
	SquareCap,
	RoundCap
};

enum LineJoin {
	MiterJoin,
	MiterJoinRevert,
	RoundJoin,
	BevelJoin,
	MiterJoinRound
};

enum InnerJoin {
	InnerBevel,
	InnerMiter,
	InnerJag,
	InnerRound
};

enum PictureFilter {
	FilterNearest,
	FilterBilinear,
	FilterHanning,
	FilterHamming,
	FilterHermite,
	FilterQuadric,
	FilterBicubic,
	FilterKaiser,
	FilterCatrom,
	FilterMitchell,
	FilterSpline16,
	FilterSpline36,
	FilterGaussian,
	FilterBessel,
	FilterSinc36,
	FilterSinc64,
	FilterSinc256,
	FilterLanczos36,
	FilterLanczos64,
	FilterLanczos256,
	FilterBlackman36,
	FilterBlackman64,
	FilterBlackman256
};
	
struct IPictureRenderer;

// A two dimensional array of colors.
class Picture {
public:
	template <typename T>
	friend struct AGGRenderer; // antigrain geometry renderer

	Picture();
	Picture(uint16_t width, uint16_t height, PictureFormat format, const void *data = nullptr);
	Picture(const Picture &source);
	~Picture();

	uint16_t GetWidth() const { return width; }
	uint16_t GetHeight() const { return height; }
	Vector2 GetCenter() const { return Vector2(float(width) * 0.5f, float(height) * 0.5f); }
	size_t GetStride() const;
	PictureFormat GetFormat() const { return format; }

	iRect GetRect() const { return iRect(0, 0, width, height); }

	/// Allocate storage for a given picture size and format.
	bool AllocAs(uint16_t width, uint16_t height, PictureFormat format);
	/// Allocate storage according to another picture.
	bool AllocAs(const Picture &as) { return AllocAs(as.GetWidth(), as.GetHeight(), as.GetFormat()); }
	/// Free storage.
	void Free();

	/// Perform a copy of an input picture.
	Picture &operator=(const Picture &source);

	/// Return picture raster data.
	uint8_t *GetData() const { return (uint8_t *)data.data(); }
	uint8_t *GetDataAt(uint16_t x, uint16_t y) const;

	size_t GetDataSize() const { return memory_usage(data); }

	/// Clear the clipping rect.
	void ClearClipping();
	/// Set the clipping rect.
	void SetClipping(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);

	/// Clear the picture with a RGBA color.
	void ClearRGBA(float r, float g, float b, float a = 1.f);
	/// Read a pixel from the picture, return as an RGBA vector.
	Vector4 GetPixelRGBA(uint16_t x, uint16_t y) const;
	/// Write a RGBA color to the picture.
	void PutPixelRGBA(uint16_t x, uint16_t y, float r, float g, float b, float a = 1.f);
	/// Blend a RGBA color with the existing content of the picture.
	void BlendPixelRGBA(uint16_t x, uint16_t y, float r, float g, float b, float a = 1.f);

	void SetFillColorRGBA(float r, float g, float b, float a = 1.f);
	void SetPenColorRGBA(float r, float g, float b, float a = 1.f);

	/// Blit copy a picture.
	bool BlitCopy(const Picture &src, iRect src_rect, iVector2 dst_pos);
	/// Blit a picture.
	bool Blit(const Picture &src, iRect src_rect, iVector2 dst_pos);
	/// Blit part of a picture.
	bool Blit(const Picture &src, iRect src_rect, iRect dst_rect, PictureFilter filter = FilterBilinear);
	/// Blit a picture with transform.
	bool BlitTransform(const Picture &src, iRect dst_rect, const Matrix3 &matrix, PictureFilter filter = FilterBilinear);

	/// Flip the picture content.
	void Flip(bool horizontal, bool vertical);
	/// Reframe the picture content.
	bool Reframe(int32_t top, int32_t bottom, int32_t left, int32_t right);
	/// Crop the picture content.
	bool Crop(int32_t start_x, int32_t start_y, int32_t end_x, int32_t end_y);

	/// Resize the picture.
	bool Resize(uint16_t width, uint16_t height, PictureFilter filter = FilterBilinear);

	/// Convert the picture to a different format.
	bool Convert(PictureFormat format);

	void SetFillMode(BrushMode mode);
	void SetPenMode(PenMode mode);
	void SetPenWidth(float width);
	void SetPenCap(PenCap cap);
	void SetLineJoin(LineJoin join);
	void SetInnerJoin(InnerJoin join);

	/// Move the path cursor to an absolute position in pixels.
	void MoveTo(float x, float y);
	/// Define a path line from the current cursor position.
	void LineTo(float x, float y);
	/// Close the current polygon.
	void ClosePolygon();
	/// Add a rounded rectangle to the path.
	void AddRoundedRect(float start_x, float start_y, float end_x, float end_y, float radius);
	/// Add an ellipse to the path.
	void AddEllipse(float x, float y, float radius_x, float radius_y);
	/// Add a circle to the path.
	void AddCircle(float x, float y, float radius);
	/// Draw path.
	void DrawPath();

	/// Draw a line.
	void DrawLine(float start_x, float start_y, float end_x, float end_y);
	/// Draw a rectangle.
	void DrawRect(float start_x, float start_y, float end_x, float end_y);
	/// Draw a rounded rectangle.
	void DrawRoundedRect(float start_x, float start_y, float end_x, float end_y, float radius);
	/// Draw an ellipse.
	void DrawEllipse(float x, float y, float radius_x, float radius_y);
	/// Draw a circle.
	void DrawCircle(float x, float y, float radius) { DrawEllipse(x, y, radius, radius); }

	/// Draw glyph.
	void DrawGlyph(FontEngine &font_engine, std::u32string::value_type glyph_code, float x, float y);
	/// Draw text.
	void DrawText(FontEngine &font_engine, const std::string &text, float x, float y);

	/// Compare this picture content with the content from another picture.
	bool Compare(const Picture &picture, float threshold = 0.f);

private:
	std::unique_ptr<IPictureRenderer> renderer;

	std::vector<char> data;
	uint16_t width, height;
	PictureFormat format : 8;
};

/// Return the pixel size in bytes (bpp/8) of a pixel format.
size_t GetPixelSize(PictureFormat format);
/// Return the number of component in a pixel format.
uint32_t GetComponentCount(PictureFormat format);

} // namespace hg
