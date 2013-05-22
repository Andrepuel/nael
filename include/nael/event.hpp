#pragma once

#include <nael/misc.hpp>

namespace nael {

struct MouseEvent {
	/**
	 * What kind of mouse event: Mouse movement, mouse button press, or mouse button released
	 */
	enum {
		MOVE,PRESS,RELEASE
	} type;
	/**
	 * Which button was pressed or released. Makes senses only if the event type is related to
	 * mouse button press/release.
	 */
	enum {
		LEFT_BUTTON,MIDDLE_BUTTON,RIGHT_BUTTON
	} button;

	/**
	 * Position of the mouse pointer when the event ocurred.
	 * @note The top left position is (0,0).
	 */
	unsigned posX, posY;
};

struct KeyboardEvent {
	/**
	 * What kind of keyboard event: Key pressed or key relase
	 */
	enum {
		KEYUP,KEYDOWN
	} type;

	/**
	 * Which key was pressed/released
	 */
	enum Key {
		KEY_BACKSPACE, KEY_TAB, KEY_CLEAR, KEY_RETURN, KEY_PAUSE, KEY_ESCAPE , KEY_SPACE, KEY_EXCLAIM, KEY_QUOTEDBL, KEY_HASH,
		KEY_DOLLAR , KEY_AMPERSAND , KEY_QUOTE , KEY_LEFTPAREN, KEY_RIGHTPAREN , KEY_ASTERISK, KEY_PLUS, KEY_COMMA, KEY_MINUS, 
		KEY_PERIOD, KEY_SLASH, KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_COLON, KEY_SEMICOLON, 
		KEY_LESS, KEY_EQUALS, KEY_GREATER, KEY_QUESTION, KEY_AT, KEY_LEFTBRACKET , KEY_BACKSLASH, KEY_RIGHTBRACKET, KEY_CARET, 
		KEY_UNDERSCORE, KEY_BACKQUOTE , KEY_a , KEY_b, KEY_c , KEY_d, KEY_e , KEY_f, KEY_g, KEY_h, KEY_i, KEY_j, KEY_k, KEY_l, 
		KEY_m, KEY_n, KEY_o, KEY_p, KEY_q, KEY_r, KEY_s, KEY_t, KEY_u, KEY_v, KEY_w, KEY_x, KEY_y, KEY_z, KEY_DELETE, KEY_KP0, 
		KEY_KP1, KEY_KP2 , KEY_KP3 , KEY_KP4 , KEY_KP5 , KEY_KP6 , KEY_KP7 , KEY_KP8 , KEY_KP9 , KEY_KP_PERIOD, KEY_KP_DIVIDE, 
		KEY_KP_MULTIPLY , KEY_KP_MINUS , KEY_KP_PLUS , KEY_KP_ENTER , KEY_KP_EQUALS , KEY_UP , KEY_DOWN , KEY_RIGHT, KEY_LEFT, 
		KEY_INSERT, KEY_HOME, KEY_END, KEY_PAGEUP, KEY_PAGEDOWN, KEY_F1 , KEY_F2 , KEY_F3 , KEY_F4 , KEY_F5 , KEY_F6 , KEY_F7, 
		KEY_F8 , KEY_F9 , KEY_F10 , KEY_F11 , KEY_F12 , KEY_F13 , KEY_F14 , KEY_F15, KEY_NUMLOCK, KEY_CAPSLOCK, KEY_SCROLLOCK, 
		KEY_RSHIFT , KEY_LSHIFT , KEY_RCTRL , KEY_LCTRL , KEY_RALT , KEY_LALT , KEY_RMETA , KEY_LMETA, KEY_LSUPER, KEY_RSUPER, 
		KEY_MODE, KEY_HELP, KEY_PRINT, KEY_SYSREQ, KEY_BREAK, KEY_MENU, KEY_POWER, KEY_EURO
	} key;

	enum Modifier {
		NONE=0,
		SHIFT=1,
		CONTROL=2,
		ALT=4
	};
	/**
	 * Which modifier key was held when the event ocurred.
	 * More than one modifier may be flagged, so comparation must use bitwise or.
	 */
	unsigned modifiers;
};

}
