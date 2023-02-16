// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/color.h"
#include "foundation/matrix4.h"
#include "foundation/rect.h"
#include "foundation/vector2.h"
#include "foundation/vector3.h"
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace hg {

struct Texture;
struct RasterFont;

class Renderer;
class RenderSystem;
class RenderGeometry;

enum CullMode {
	CullBack,
	CullFront,
	CullNever
};

enum BlendMode {
	BlendOpaque,
	BlendAlpha,
	BlendAdditive
};

class SimpleGraphicEngine {
public:
	void SetSnapGlyphToGrid(bool v) { snap_glyph_to_grid = v; }
	bool GetSnapGlyphToGrid() const { return snap_glyph_to_grid; }

	void SetBlendMode(BlendMode mode);
	BlendMode GetBlendMode() const;
	void SetCullMode(CullMode mode);
	CullMode GetCullMode() const;

	void SetDepthWrite(bool write);
	bool GetDepthWrite() const;
	void SetDepthTest(bool test);
	bool GetDepthTest() const;

	void Line(float sx, float sy, float sz, float ex, float ey, float ez, const Color &start_color, const Color &end_color);
	void Triangle(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, const Color &a_color, const Color &b_color, const Color &c_color);
	void Text(float x, float y, float z, const std::string &text, const Color &color, const std::shared_ptr<RasterFont> &font, float scale = 1.f);
	void Text(const Matrix4 &mat, const std::string &text, const Color &color, const std::shared_ptr<RasterFont> &font, float scale = 1.f);
	void Quad(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, float dx, float dy, float dz, float uv_sx, float uv_sy, float uv_ex, float uv_ey, std::shared_ptr<Texture> tex, const Color &a_color, const Color &b_color, const Color &c_color, const Color &d_color);
	void Geometry(float x, float y, float z, float ex, float ey, float ez, float sx, float sy, float sz, std::shared_ptr<RenderGeometry> geo);

	void Draw(RenderSystem &render_system);
	void Clear(RenderSystem &render_system);

	/// Draw and clear the engine queue in one call.
	void Flush(RenderSystem &render_system);

private:
	mutable std::mutex engine_mutex;

	void Text_LowLevel(const Matrix4 &mat, const std::string &text, const Color &color, const std::shared_ptr<RasterFont> &font, float scale = 1.f, bool orient_toward_camera = true);

	enum BatchType {
		BatchEmpty,
		BatchLine,
		BatchTriangle,
		BatchText,
		BatchQuad,
		BatchGeometry
	};

	struct RenderState {
		RenderState() : depth_write(true), depth_test(true) {}

		bool operator==(const RenderState &s) const { return blend_mode == s.blend_mode && cull_mode == s.cull_mode && depth_write == s.depth_write && depth_test == s.depth_test; }

		BlendMode blend_mode{BlendOpaque};
		CullMode cull_mode{CullFront};
		bool depth_write : 1, depth_test : 1;
	};

	template <class T>
	T *NewBatch(BatchType);

	/// Return true if the current batch modes and type are compatible with the engine modes and the required batch type.
	inline bool IsBatchCompatible(BatchType type) const { return batch && batch_type == type && batch->render_state == render_state && !batch->IsFull(); }

	struct Batch {
		virtual ~Batch() {}

		virtual void Flush(RenderSystem &render_system, Renderer &renderer) = 0;
		virtual bool IsFull() { return false; }

		RenderState render_state;
	};

	struct LineBatch : Batch {
		LineBatch();

		void Flush(RenderSystem &render_system, Renderer &renderer) override;
		bool IsFull() override { return vtx.size() >= 65534; }

		std::vector<Vector3> vtx;
		std::vector<Color> rgb;
	};

	struct TriangleBatch : Batch {
		TriangleBatch();

		void Flush(RenderSystem &render_system, Renderer &renderer) override;
		bool IsFull() override { return vtx.size() >= 65535; }

		std::vector<Vector3> vtx;
		std::vector<Color> rgb;
	};

	struct TextBatch : Batch {
		TextBatch();

		void Flush(RenderSystem &render_system, Renderer &renderer) override;

	private:
		friend class SimpleGraphicEngine;

		struct Data {
			std::shared_ptr<RasterFont> font;
			Matrix4 mat;
			std::string text;
			Color color;
			float scale;
			bool orient_toward_camera;
		};

		std::vector<Data> datas;

		bool snap_glyph_to_grid{true};
	};

	struct QuadBatch : Batch {
		QuadBatch();

		void Flush(RenderSystem &render_system, Renderer &renderer) override;
		bool IsFull() override { return idx.size() >= 65532; }

		std::shared_ptr<Texture> tex;
		std::vector<uint16_t> idx;
		std::vector<Vector3> vtx;
		std::vector<Vector2> uv;
		std::vector<Color> rgb;
	};

	struct GeometryBatch : Batch {
		void Flush(RenderSystem &render_system, Renderer &renderer) override;

		std::shared_ptr<RenderGeometry> geo;
		std::vector<Matrix4> world;
	};

	RenderState render_state;

	Batch *batch{nullptr};
	BatchType batch_type{BatchEmpty};

	std::vector<std::unique_ptr<Batch>> batches;

	void Draw_async(RenderSystem &render_system);
	void Clear_async(RenderSystem &render_system);
	void Flush_async(RenderSystem &render_system);

	bool snap_glyph_to_grid{true};
};

} // namespace hg
