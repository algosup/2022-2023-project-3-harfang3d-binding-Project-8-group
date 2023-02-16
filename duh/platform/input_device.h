// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/matrix4.h"
#include <memory>

namespace hg {

enum InputDeviceType {
	InputDeviceAny,
	InputDeviceKeyboard,
	InputDeviceMouse,
	InputDevicePad,
	InputDeviceTouch,
	InputDeviceHMD,
	InputDeviceController
};

enum Key {
	KeyLShift,
	KeyRShift,
	KeyLCtrl,
	KeyRCtrl,
	KeyLAlt,
	KeyRAlt,
	KeyLWin,
	KeyRWin,
	KeyLMenu,
	KeyRMenu,
	KeyTab,
	KeyCapsLock,
	KeySpace,
	KeyBackspace,
	KeyInsert,
	KeySuppr,
	KeyHome,
	KeyEnd,
	KeyPageUp,
	KeyPageDown,
	KeyUp,
	KeyDown,
	KeyLeft,
	KeyRight,
	KeyEscape,
	KeyF1,
	KeyF2,
	KeyF3,
	KeyF4,
	KeyF5,
	KeyF6,
	KeyF7,
	KeyF8,
	KeyF9,
	KeyF10,
	KeyF11,
	KeyF12,
	KeyPrintScreen,
	KeyScrollLock,
	KeyPause,
	KeyNumLock,
	KeyReturn,
	Key0,
	Key1,
	Key2,
	Key3,
	Key4,
	Key5,
	Key6,
	Key7,
	Key8,
	Key9,
	KeyNumpad0,
	KeyNumpad1,
	KeyNumpad2,
	KeyNumpad3,
	KeyNumpad4,
	KeyNumpad5,
	KeyNumpad6,
	KeyNumpad7,
	KeyNumpad8,
	KeyNumpad9,
	KeyAdd,
	KeySub,
	KeyMul,
	KeyDiv,
	KeyEnter,
	KeyA,
	KeyB,
	KeyC,
	KeyD,
	KeyE,
	KeyF,
	KeyG,
	KeyH,
	KeyI,
	KeyJ,
	KeyK,
	KeyL,
	KeyM,
	KeyN,
	KeyO,
	KeyP,
	KeyQ,
	KeyR,
	KeyS,
	KeyT,
	KeyU,
	KeyV,
	KeyW,
	KeyX,
	KeyY,
	KeyZ,
	KeyPlus,
	KeyComma,
	KeyMinus,
	KeyPeriod,
	KeyOEM1,
	KeyOEM2,
	KeyOEM3,
	KeyOEM4,
	KeyOEM5,
	KeyOEM6,
	KeyOEM7,
	KeyOEM8,
	KeyBrowserBack,
	KeyBrowserForward,
	KeyBrowserRefresh,
	KeyBrowserStop,
	KeyBrowserSearch,
	KeyBrowserFavorites,
	KeyBrowserHome,
	KeyVolumeMute,
	KeyVolumeDown,
	KeyVolumeUp,
	KeyMediaNextTrack,
	KeyMediaPrevTrack,
	KeyMediaStop,
	KeyMediaPlayPause,
	KeyLaunchMail,
	KeyLaunchMediaSelect,
	KeyLaunchApp1,
	KeyLaunchApp2,
	KeyLast
};

enum Button {
	Button0,
	Button1,
	Button2,
	Button3,
	Button4,
	Button5,
	Button6,
	Button7,
	Button8,
	Button9,
	Button10,
	Button11,
	Button12,
	Button13,
	Button14,
	Button15,
	Button16,
	Button17,
	Button18,
	Button19,
	Button20,
	Button21,
	Button22,
	Button23,
	Button24,
	Button25,
	Button26,
	Button27,
	Button28,
	Button29,
	Button30,
	Button31,
	Button32,
	Button33,
	Button34,
	Button35,
	Button36,
	Button37,
	Button38,
	Button39,
	Button40,
	Button41,
	Button42,
	Button43,
	Button44,
	Button45,
	Button46,
	Button47,
	Button48,
	Button49,
	Button50,
	Button51,
	Button52,
	Button53,
	Button54,
	Button55,
	Button56,
	Button57,
	Button58,
	Button59,
	Button60,
	Button61,
	Button62,
	Button63,
	Button64,
	Button65,
	Button66,
	Button67,
	Button68,
	Button69,
	Button70,
	Button71,
	Button72,
	Button73,
	Button74,
	Button75,
	Button76,
	Button77,
	Button78,
	Button79,
	Button80,
	Button81,
	Button82,
	Button83,
	Button84,
	Button85,
	Button86,
	Button87,
	Button88,
	Button89,
	Button90,
	Button91,
	Button92,
	Button93,
	Button94,
	Button95,
	Button96,
	Button97,
	Button98,
	Button99,
	Button100,
	Button101,
	Button102,
	Button103,
	Button104,
	Button105,
	Button106,
	Button107,
	Button108,
	Button109,
	Button110,
	Button111,
	Button112,
	Button113,
	Button114,
	Button115,
	Button116,
	Button117,
	Button118,
	Button119,
	Button120,
	Button121,
	Button122,
	Button123,
	Button124,
	Button125,
	Button126,
	Button127,
	ButtonBack,
	ButtonStart,
	ButtonSelect,
	ButtonL1,
	ButtonL2,
	ButtonL3,
	ButtonR1,
	ButtonR2,
	ButtonR3,
	ButtonCrossUp,
	ButtonCrossDown,
	ButtonCrossLeft,
	ButtonCrossRight,
	ButtonLast
};

enum AnalogInput {
	InputAxisX,
	InputAxisY,
	InputAxisZ,
	InputAxisS,
	InputAxisT,
	InputAxisR,
	InputRotX,
	InputRotY,
	InputRotZ,
	InputRotS,
	InputRotT,
	InputRotR,
	InputButton0,
	InputButton1,
	InputButton2,
	InputButton3,
	InputButton4,
	InputButton5,
	InputButton6,
	InputButton7,
	InputButton8,
	InputButton9,
	InputButton10,
	InputButton11,
	InputButton12,
	InputButton13,
	InputButton14,
	InputButton15,
	InputLast
};

enum InputDeviceEffect {
	InputDeviceVibrate,
	InputDeviceVibrateLeft,
	InputDeviceVibrateRight,
	InputDeviceConstantForce
};

enum InputDeviceMatrix {
	InputDeviceMatrixHead
};

struct InputDevice {
	virtual ~InputDevice() = default;

	/// Get device type.
	virtual InputDeviceType GetType() const = 0;

	/// Pool state and refresh values.
	virtual void Update() = 0;

	/// Test a key state.
	virtual bool IsDown(Key key) const { return false; }
	/// Test a key state during the previous update.
	virtual bool WasDown(Key key) const { return false; }

	inline bool WasPressed(Key key) const { return !WasDown(key) && IsDown(key); }
	inline bool WasReleased(Key key) const { return WasDown(key) && !IsDown(key); }

	virtual const char *GetKeyName(Key key) const { return ""; }

	/// Test a button state.
	virtual bool IsButtonDown(Button btn) const { return false; }
	/// Test a button state during the previous update.
	virtual bool WasButtonDown(Button btn) const { return false; }

	inline bool WasButtonPressed(Button btn) const { return !WasButtonDown(btn) && IsButtonDown(btn); }
	inline bool WasButtonReleased(Button btn) const { return WasButtonDown(btn) && !IsButtonDown(btn); }

	/// Get input value.
	virtual float GetValue(AnalogInput inp) const { return 0.f; }
	/// Get input value during last update.
	virtual float GetLastValue(AnalogInput inp) const { return 0.f; }
	/// Get input value range.
	virtual bool GetValueRange(AnalogInput inp, float &min, float &max) const { return false; }

	/// Return input value delta since last update.
	inline float GetDelta(AnalogInput inp) const { return GetValue(inp) - GetLastValue(inp); }

	/// Return a device matrix.
	virtual Matrix4 GetMatrix(InputDeviceMatrix mtx) const { return Matrix4::Identity; }

	/// Set input value, return true if the device supports setting the value.
	virtual bool SetValue(AnalogInput inp, float v) { return false; }

	/// Set device effect.
	virtual bool SetEffect(InputDeviceEffect effect, float v) { return false; }

	/// Return true if the device is connected.
	virtual bool IsConnected() { return true; }
};

} // namespace hg
