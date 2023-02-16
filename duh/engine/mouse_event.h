// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

namespace hg {

enum MouseEventType {
	MouseEventNone,
	MouseEventMove,
	MouseEventClick
};

enum MouseEventButton {
	MouseEventNoButton = 0x0,
	MouseEventLeftButton = 0x1,
	MouseEventRightButton = 0x2,
	MouseEventMiddleButton = 0x4
};

struct MouseState {
	uint8_t buttons{MouseEventNoButton};
	float x{0}, y{0}, wheel{0};
};

struct MouseEvent {
	uint8_t type{MouseEventNone};
	MouseState state, last_state;
};

} // namespace hg
