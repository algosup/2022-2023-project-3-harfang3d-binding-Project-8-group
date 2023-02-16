// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/light.h"
#include "engine/simple_graphic_engine.h"
#include "foundation/clock.h"
#include "foundation/global_object.h"
#include "foundation/matrix3.h"
#include "platform/input_device.h"
#include "platform/window_system.h"

namespace hg {

struct RenderWindow {
	Window window;
	Surface surface;
};

bool operator==(const RenderWindow &a, const RenderWindow &b);

struct Mixer;
class MixerAsync;

class Renderer;
class RendererAsync;
class RenderSystem;
class RenderSystemAsync;

struct RasterFont;

class RigidBody;
class Environment;

class Scene;

class Geometry;

/// Provide the highest-level of abstraction of the Harfang API
class Plus {
public:
	Plus();
	~Plus();

	/// Start the worker pool.
	void CreateWorkers();
	/// Stop the worker pool.
	void DeleteWorkers();

	/// Mount access to the file system.
	void Mount();
	void Mount(const std::string &path);
	void MountAs(const std::string &path, const std::string &prefix);

	void Unmount(const std::string &path);
	void UnmountAll();

	const std::shared_ptr<Renderer> &GetRenderer() const { return renderer; }
	const std::shared_ptr<RendererAsync> &GetRendererAsync() const { return renderer_async; }

	const std::shared_ptr<RenderSystem> &GetRenderSystem() const { return render_system; }
	const std::shared_ptr<RenderSystemAsync> &GetRenderSystemAsync() const { return render_system_async; }

	enum AppEndCondition {
		EndOnEscapePressed = 0x1,
		EndOnDefaultWindowClosed = 0x2,
		EndOnAny = 0xffff
	};

	/// Test if the application has ended.
	bool IsAppEnded(AppEndCondition flag = EndOnAny) const;

	/**
		@name Rendering
		@{
	*/
public:
	/// Initialize the rendering engine.
	bool RenderInit(int width, int height, int aa = 1, Window::Visibility visibility = Window::Windowed, bool debug = false);
	bool RenderInit(int width, int height, const std::string &core_path, int aa = 1, Window::Visibility visibility = Window::Windowed, bool debug = false);
	/// Shutdown the rendering engine.
	void RenderUninit();

	/// Create a new output window.
	RenderWindow NewRenderWindow(int width, int height, Window::Visibility visibility = Window::Windowed);
	/// Delete output window.
	void FreeRenderWindow(RenderWindow &window);

	/// Get current output window.
	RenderWindow GetRenderWindow() const { return current_window; }
	/// Set current output window.
	void SetRenderWindow(const RenderWindow &window);
	/// Set default render window as the current output window.
	void SetRenderWindow();

	/// Get render window size.
	iVector2 GetRenderWindowSize(const RenderWindow &window);
	/// Update a render window.
	void UpdateRenderWindow(const RenderWindow &window);

	/// Set the default render window title.
	void SetWindowTitle(const std::string &title);
	/// Set render window title.
	void SetWindowTitle(const RenderWindow &window, const std::string &title);

	/// Initialize the rendering engine from externally created objects.
	void InitExtern(std::shared_ptr<Renderer> renderer, std::shared_ptr<RendererAsync> renderer_async, std::shared_ptr<RenderSystem> render_system, std::shared_ptr<RenderSystemAsync> render_system_async);
	/// Shutdown the rendering engine when initialized from externally created objects.
	void UninitExtern();

	/// Select between a top-left or bottom-left origin for 2D drawing.
	void Set2DOriginIsTopLeft(bool top_left) { origin_2d_is_top_left = top_left; }
	/// Set fixed 2D resolution.
	void SetFixed2DResolution(float w, float h) { fixed_2d_res = {w, h}; }
	/// Get fixed 2D resolution.
	Vector2 GetFixed2DResolution() const { return fixed_2d_res; }

	/// Commit all 2D calls.
	void Commit2D();
	/// Commit all 3D calls.
	void Commit3D();

	/// Return the system screen width.
	int GetScreenWidth() const;
	/// Return the system screen height.
	int GetScreenHeight() const;

	/// Draw and display frame.
	void Flip();

	/// Signal end of frame.
	void EndFrame();

	/// Wait for the rendering thread to complete its queued work before returning.
	void WaitForRender();

	/// Set the 2D blend mode.
	void SetBlend2D(BlendMode mode) { graphic_engine_2d.SetBlendMode(mode); }
	/// Get the 2D blend mode.
	BlendMode GetBlend2D() const { return graphic_engine_2d.GetBlendMode(); }
	/// Set the 2D cull mode.
	void SetCulling2D(CullMode mode) { graphic_engine_2d.SetCullMode(mode); }
	/// Set the 2D cull mode.
	CullMode GetCulling2D() const { return graphic_engine_2d.GetCullMode(); }

	/// Set the current 3D blend mode.
	void SetBlend3D(BlendMode mode) { graphic_engine_3d.SetBlendMode(mode); }
	/// Get the current 3D blend mode.
	BlendMode GetBlend3D() const { return graphic_engine_3d.GetBlendMode(); }
	/// Set the 3D cull mode.
	void SetCulling3D(CullMode mode) { graphic_engine_3d.SetCullMode(mode); }
	/// Set the 3D cull mode.
	CullMode GetCulling3D() const { return graphic_engine_3d.GetCullMode(); }

	/// Enable/disable depth testing 2D.
	void SetDepthTest2D(bool enable) { graphic_engine_2d.SetDepthTest(enable); }
	/// Return the 2D depth test state.
	bool GetDepthTest2D() const { return graphic_engine_2d.GetDepthTest(); }
	/// Enable/disable depth writing in 2D.
	void SetDepthWrite2D(bool enable) { graphic_engine_2d.SetDepthWrite(enable); }
	/// Return the 2D depth writing state.
	bool GetDepthWrite2D() const { return graphic_engine_2d.GetDepthWrite(); }

	/// Enable/disable depth testing 3D.
	void SetDepthTest3D(bool enable) { graphic_engine_3d.SetDepthTest(enable); }
	/// Return the 3D depth test state.
	bool GetDepthTest3D() const { return graphic_engine_3d.GetDepthTest(); }
	/// Enable/disable depth writing in 3D.
	void SetDepthWrite3D(bool enable) { graphic_engine_3d.SetDepthWrite(enable); }
	/// Return the 3D depth writing state.
	bool GetDepthWrite3D() const { return graphic_engine_3d.GetDepthWrite(); }

	/// Clear.
	void Clear(Color color = Color::Black);

	/// Draw a 2D point.
	void Plot2D(float x, float y, Color color = Color::White) { graphic_engine_2d.Line(x, y, 0, x + 1, y + 1, 0, color, color); }
	/// Draw a 2D line.
	void Line2D(float sx, float sy, float ex, float ey, Color start_color = Color::White, Color end_color = Color::White) { graphic_engine_2d.Line(sx, sy, 0, ex, ey, 0, start_color, end_color); }
	/// Draw a 2D triangle.
	void Triangle2D(float ax, float ay, float bx, float by, float cx, float cy, Color a_color = Color::White, Color b_color = Color::White, Color c_color = Color::White) { graphic_engine_2d.Triangle(ax, ay, 0, bx, by, 0, cx, cy, 0, a_color, b_color, c_color); }
	/// Draw a 2D quad.
	void Quad2D(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy, Color a_color = Color::White, Color b_color = Color::White, Color c_color = Color::White, Color d_color = Color::White, std::shared_ptr<Texture> tex = nullptr, float uv_sx = 0, float uv_sy = 0, float uv_ex = 1, float uv_ey = 1) { graphic_engine_2d.Quad(ax, ay, 0, bx, by, 0, cx, cy, 0, dx, dy, 0, uv_sx, uv_sy, uv_ex, uv_ey, tex, a_color, b_color, c_color, d_color); }

	/// Draw a 3D line.
	void Line3D(float sx, float sy, float sz, float ex, float ey, float ez, Color start_color = Color::White, Color end_color = Color::White) { graphic_engine_3d.Line(sx, sy, sz, ex, ey, ez, start_color, end_color); }
	/// Draw a 3D triangle.
	void Triangle3D(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, Color a_color = Color::White, Color b_color = Color::White, Color c_color = Color::White) { graphic_engine_3d.Triangle(ax, ay, az, bx, by, bz, cx, cy, cz, a_color, b_color, c_color); }
	/// Draw a 3D quad.
	void Quad3D(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, float dx, float dy, float dz, Color a_color = Color::White, Color b_color = Color::White, Color c_color = Color::White, Color d_color = Color::White, std::shared_ptr<Texture> tex = nullptr, float uv_sx = 0, float uv_sy = 0, float uv_ex = 1, float uv_ey = 1) { graphic_engine_3d.Quad(ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz, uv_sx, uv_sy, uv_ex, uv_ey, tex, a_color, b_color, c_color, d_color); }

private:
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<RendererAsync> renderer_async;

	std::shared_ptr<RenderSystem> render_system;
	std::shared_ptr<RenderSystemAsync> render_system_async;

	SimpleGraphicEngine graphic_engine_2d;
	SimpleGraphicEngine graphic_engine_3d;

	bool hack_vsync_off{false};

	RenderWindow current_window, default_window;
	bool is_default_window_open{false};

	void OnDestroyWindow(const Window &window);
	Signal<void(const Window &)>::Connection destroy_window_connection;

	bool origin_2d_is_top_left{false};

	void Apply3D();

	bool RenderInit_CreateRenderObjects(bool debug);
	bool RenderInit_EndInit(const std::string &core_path, int aa);

	std::atomic<int> width{0}, height{0};

	Vector2 fixed_2d_res{0, 0};

	std::future<void> flip_future;

public:
	/// Set the current font path.
	void SetFont(const std::string &path) { current_font_path = path; }
	/// Get the current font path.
	const std::string &GetFont() const { return current_font_path; }

	/// Draw 2D text.
	void Text2D(float x, float y, const std::string &text, float size = 16, Color color = Color::White, const std::string &font_path = std::string());
	/// Draw 3D text.
	void Text3D(float x, float y, float z, const std::string &text, float size = 16, Color color = Color::White, const std::string &font_path = std::string());

	/// Return a text rect.
	fRect GetTextRect(const std::string &text, float size = 16, const std::string &font_path = std::string());

	/// Draw a sprite.
	void Sprite2D(float x, float y, float size, const std::string &image_path, Color color = Color::White, float pivot_x = 0.5, float pivot_y = 0.5, bool flip_h = false, bool flip_v = false);
	/// Draw a rotated sprite.
	void RotatedSprite2D(float x, float y, float rotation, float size, const std::string &image_path, Color color = Color::White, float pivot_x = 0.5, float pivot_y = 0.5, bool flip_h = false, bool flip_v = false);
	/// Draw an image.
	void Image2D(float x, float y, float scale, const std::string &image_path, Color color = Color::White);
	/// Draw an image rect.
	void Blit2D(float src_x, float src_y, float src_w, float src_h, float dst_x, float dst_y, float dst_w, float dst_h, const std::string &image_path, Color color = Color::White);
	/// Draw a texture.
	void Texture2D(float x, float y, float scale, const std::shared_ptr<Texture> &tex, Color color = Color::White, bool flip_h = false, bool flip_v = false);

	/// Load a texture from its path.
	std::shared_ptr<Texture> LoadTexture(const std::string &path);
	/// Load a render material from its path.
	std::shared_ptr<RenderMaterial> LoadMaterial(const std::string &path);
	std::shared_ptr<RenderMaterial> LoadMaterial(const std::string &path, const std::string &source);
	/// Load a surface shader from its path.
	std::shared_ptr<SurfaceShader> LoadSurfaceShader(const std::string &path);
	std::shared_ptr<SurfaceShader> LoadSurfaceShader(const std::string &path, const std::string &source);
	/// Load a render geometry from its path.
	std::shared_ptr<RenderGeometry> LoadGeometry(const std::string &path);
	/// Create a render geometry from a core geometry.
	std::shared_ptr<RenderGeometry> CreateGeometry(const std::shared_ptr<Geometry> &core_geo, bool use_cache = true);

	/// Draw a geometry in 2D.
	void Geometry2D(float x, float y, std::shared_ptr<RenderGeometry> geo, float angle_x = 0, float angle_y = 0, float angle_z = 0, float scale = 1);
	/// Draw a geometry in 3D.
	void Geometry3D(float x, float y, float z, std::shared_ptr<RenderGeometry> geo, float angle_x = 0, float angle_y = 0, float angle_z = 0, float scale = 1);

private:
	std::string current_font_path = "@core/fonts/default.ttf";

	std::map<std::string, std::shared_ptr<RasterFont>> fonts;

	std::shared_ptr<RasterFont> CacheFont(const std::string &path, float size);

	struct TextRenderState {
		BlendMode blend_mode;
		bool depth_test;
	};

	TextRenderState ValidateTextRenderState(SimpleGraphicEngine &engine);
	void RestoreRenderState(SimpleGraphicEngine &engine, const TextRenderState &state);

public:
	/// Set the render system 3D camera properties.
	void SetCamera3D(float x, float y, float z, float angle_x = 0, float angle_y = 0, float angle_z = 0, float fov = 60, float znear = 0.1, float zfar = 1000);
	void SetCamera3D(const Matrix4 &view, const Matrix44 &projection);

	Matrix4 GetCamera3DMatrix() const { return camera3d_state.view; }
	Matrix44 GetCamera3DProjectionMatrix() const { return camera3d_state.projection; }

private:
	struct Camera3DState {
		Matrix4 view;
		Matrix4 previous_view;
		Matrix44 projection;
	};

	Camera3DState camera3d_state;
	/// @}

	/**
		@name Audio
		@{
	*/
public:
	/// Initialize the audio engine.
	bool AudioInit();
	/// Uninitialize the audio engine.
	void AudioUninit();

	/// Return the mixer.
	std::shared_ptr<Mixer> GetMixer() const { return mixer; }
	/// Return the asynchronous mixer.
	std::shared_ptr<MixerAsync> GetMixerAsync() const { return mixer_async; }

private:
	std::shared_ptr<Mixer> mixer;
	std::shared_ptr<MixerAsync> mixer_async;
	/// @}

	/**
		@name Input device
		@{
	*/
public:
	/// Return the mouse input device.
	std::shared_ptr<InputDevice> GetMouse() const;
	/// Return the keyboard input device.
	std::shared_ptr<InputDevice> GetKeyboard() const;

	/// Return the mouse position.
	void GetMousePos(float &x, float &y) const;
	/// Return the mouse position change since last update.
	void GetMouseDt(float &x, float &y) const;

	/// Return True if the mouse button is down.
	bool MouseButtonDown(Button btn = Button0) const;
	/// Return True if a key is down.
	bool KeyDown(Key key) const;
	/// Return True if a key was pressed.
	bool KeyPress(Key key) const;
	/// Return True if a key was released.
	bool KeyReleased(Key key) const;
	/// @}

	/**
		@name Clock
		@{
	*/
public:
	/// Reset the elapsed time counter.
	void ResetClock();
	/// Update clock and return the elapsed time since last update in second.
	time_ns UpdateClock();

	/// Return elapsed time since last update in second.
	time_ns GetClockDt() const;
	/// Return the total elapsed time since the object creation or the last call to Reset.
	time_ns GetClock() const;

private:
	Clock clock;
	/// @}

	/**
		@name Geometry
		@{
	*/
public:
	/// Create a capsule geometry.
	std::shared_ptr<Geometry> CreateCapsule(float radius = 0.5f, float height = 2, int subdiv_x = 6, int subdiv_y = 16, const std::string &material_path = std::string(), const std::string &name = std::string());
	/// Create a cone geometry.
	std::shared_ptr<Geometry> CreateCone(float radius = 0.5f, float height = 1, int subdiv_x = 16, const std::string &material_path = std::string(), const std::string &name = std::string());
	/// Create a cube geometry.
	std::shared_ptr<Geometry> CreateCube(float width = 1, float height = 1, float length = 1, const std::string &material_path = std::string(), const std::string &name = std::string());
	/// Create a cylinder geometry.
	std::shared_ptr<Geometry> CreateCylinder(float radius = 0.5f, float height = 1, int subdiv_x = 16, const std::string &material_path = std::string(), const std::string &name = std::string());
	/// Create a plane geometry.
	std::shared_ptr<Geometry> CreatePlane(float width = 1, float length = 1, int subdiv = 1, const std::string &material_path = std::string(), const std::string &name = std::string());
	/// Create a sphere geometry.
	std::shared_ptr<Geometry> CreateSphere(float radius = 0.5f, int subdiv_x = 6, int subdiv_y = 16, const std::string &material_path = std::string(), const std::string &name = std::string());
	/// Create a geometry from heightmap.
	std::shared_ptr<Geometry> CreateGeometryFromHeightmap(uint32_t width, uint32_t height, const std::vector<float> &heightmap, float scale = 1, const std::string &material_path = std::string(), const std::string &name = std::string());

private:
	std::string default_material_path{"@core/materials/default.mat"};
	/// @}

	/**
		@name Scene
		@{
	*/
public:
	/// Create a new scene.
	std::shared_ptr<Scene> NewScene(bool use_physics = true, bool use_lua = true);
	/// Load a scene.
	bool LoadScene(Scene &scene, const std::string &path);

	/// Update scene.
	void UpdateScene(Scene &scene, time_ns dt);
	void UpdateScene(Scene &scene);

	/// Render scene, do not perform any system update. NOTE: UpdateScene already renders the scene, use this function when you need to render additional points of view of the current scene state.
	void RenderScene(Scene &scene);

	/// Create a new dummy node and add it to a scene.
	std::shared_ptr<Node> AddDummy(Scene &scene, Matrix4 mat = Matrix4::Identity);
	/// Create a new camera node and add it to a scene.
	std::shared_ptr<Node> AddCamera(Scene &scene, Matrix4 mat = Matrix4::Identity, bool orthographic = false, bool set_as_current = true);
	/// Create a new light node and add it to a scene.
	std::shared_ptr<Node> AddLight(Scene &scene, Matrix4 mat = Matrix4::Identity, Light::Model model = Light::ModelPoint, float range = 0.f, bool shadow = true, Color diffuse = Color(0.8f, 0.8f, 0.8f), Color specular = Color(0.5f, 0.5f, 0.5f));
	/// Create a new object node and add it to a scene.
	std::shared_ptr<Node> AddObject(Scene &scene, std::shared_ptr<RenderGeometry> geo, Matrix4 mat = Matrix4::Identity, bool is_static = false);
	/// Create a new object node and add it to a scene.
	std::shared_ptr<Node> AddGeometry(Scene &scene, const std::string &geo_name, Matrix4 mat = Matrix4::Identity);
	/// Create a new plane node and add it to a scene.
	std::shared_ptr<Node> AddPlane(Scene &scene, Matrix4 mat = Matrix4::Identity, float width = 1000.f, float depth = 1000.f, const std::string &material_path = std::string(), bool use_geometry_cache = true);
	/// Create a new cube node and add it to a scene.
	std::shared_ptr<Node> AddCube(Scene &scene, Matrix4 mat = Matrix4::Identity, float width = 1.f, float height = 1.f, float depth = 1.f, const std::string &material_path = std::string(), bool use_geometry_cache = true);
	/// Create a new sphere node and add it to a scene.
	std::shared_ptr<Node> AddSphere(Scene &scene, Matrix4 mat = Matrix4::Identity, float radius = 0.5f, int subdiv_x = 6, int subdiv_y = 16, const std::string &material_path = std::string(), bool use_geometry_cache = true);
	/// Create a new geometry node from an heightmap and add it to a scene.
	std::shared_ptr<Node> AddGeometryFromHeightmap(Scene &scene, Matrix4 mat, int width, int height, const std::vector<float> &heightmap, float scale = 1.f, const std::string &material_path = std::string());
	/// Create a new environment component and add it to a scene.
	std::shared_ptr<Environment> AddEnvironment(Scene &scene, Color bg_color = Color::Black, Color ambient_color = Color::Black, Color fog_color = Color::Black, float fog_near = 0.f, float fog_far = 0.f);
	/// Create a new cube node, configure its physic attributes, and add it to a scene.
	std::shared_ptr<Node> AddPhysicCube(std::shared_ptr<RigidBody> &out_rigid_body, Scene &scene, Matrix4 mat = Matrix4::Identity, float width = 1.f, float height = 1.f, float depth = 1.f, float mass = 1.f, const std::string &material_path = std::string());
	/// Create a new plane node, configure its physic attributes, and add it to a scene.
	std::shared_ptr<Node> AddPhysicPlane(std::shared_ptr<RigidBody> &out_rigid_body, Scene &scene, Matrix4 mat = Matrix4::Identity, float width = 1000.f, float length = 1000.f, float mass = 0.f, const std::string &material_path = std::string());
	/// Create a new sphere node, configure its physic attributes, and add it to a scene.
	std::shared_ptr<Node> AddPhysicSphere(std::shared_ptr<RigidBody> &out_rigid_body, Scene &scene, Matrix4 mat = Matrix4::Identity, float radius = 0.5f, int subdiv_x = 6, int subdiv_y = 16, float mass = 1.f, const std::string &material_path = std::string());
	/// @}
};

/// Orbital controls over a Node object
class OrbitalController {
public:
	void ApplyToNode(Node &node);

	void Update(time_ns dt);
	void UpdateAndApplyToNode(Node &node, time_ns dt);

	void Reset(Vector3 _tgt, float _d) {
		tgt = _tgt;
		d = _d;
	}

	float GetSpeed() const { return k_orbit_speed; }
	void SetSpeed(float s) { k_orbit_speed = s; }
	float GetZoomSpeed() const { return k_wheel; }
	void GetZoomSpeed(float t) { k_wheel = t; }
private:
	Vector3 tgt;
	Matrix3 rot;
	float d{10};

	float k_orbit_speed{1.5f};
	float k_wheel{20};
};

/// First-person controls over a Node object
class FPSController {
public:
	FPSController(float x = 0, float y = 0, float z = 0, float _speed = 10, float _turbo = 4) : pos(x, y, z), rot(0, 0, 0), raw_pos(x, y, z), raw_rot(0, 0, 0), speed(_speed), turbo(_turbo) {}

	void Reset(Vector3 _pos, Vector3 _rot) {
		pos = raw_pos = _pos;
		rot = raw_rot = _rot;
	}

	void SetSmoothFactor(float k_pos, float k_rot);

	void ApplyToNode(Node &node);

	void Update(time_ns dt);
	void UpdateAndApplyToNode(Node &node, time_ns dt);

	Vector3 GetPos() const { return pos; }
	Vector3 GetRot() const { return rot; }

	void SetPos(const Vector3 &v) { raw_pos = pos = v; }
	void SetRot(const Vector3 &v) { raw_rot = rot = v; }

	float GetSpeed() const { return speed; }
	void SetSpeed(float s) { speed = s; }
	float GetTurbo() const { return turbo; }
	void SetTurbo(float t) { turbo = t; }

private:
	Vector3 pos, rot;
	Vector3 raw_pos, raw_rot;

	float speed, turbo;
	float smooth_pos{1}, smooth_rot{1};
};

/// Global plus object.
extern global_object<Plus> g_plus;

} // namespace hg
