/*
 * Mage3D
 * Copyright (C) 2020 Blue Moon Development. All rights reserved.
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: team@bluemoondev.org
 * 
 * File Name: input.h
 * Date File Created: 7/27/2020 at 4:38 PM
 * Author: Matt
 */

#ifndef MAGE3D_INPUT_H
#define MAGE3D_INPUT_H

#include "mage3d_exported.h"
#include "mage/common.h"

#define MAX_KEYS 1024
#define MAX_BUTTONS 32

// DEFINES TAKEN FROM GLFW3.H
// ONLY CHANGE: STRIPPED GLFW_ FROM NAMES FOR QUALITY OF LIFE SAKE

//@formatter:off
#define HAT_CENTERED                    	GLFW_HAT_CENTERED
#define HAT_UP                            	GLFW_HAT_UP
#define HAT_RIGHT                        	GLFW_HAT_RIGHT
#define HAT_DOWN                        	GLFW_HAT_DOWN
#define HAT_LEFT                        	GLFW_HAT_LEFT
#define HAT_RIGHT_UP                    	(GLFW_HAT_RIGHT | GLFW_HAT_UP)
#define HAT_RIGHT_DOWN                    	(GLFW_HAT_RIGHT | GLFW_HAT_DOWN)
#define HAT_LEFT_UP                        	(GLFW_HAT_LEFT  | GLFW_HAT_UP)
#define HAT_LEFT_DOWN                    	(GLFW_HAT_LEFT  | GLFW_HAT_DOWN)

/* The unknown key */
#define KEY_UNKNOWN                        	GLFW_KEY_UNKNOWN

/* Printable keys */
#define KEY_SPACE                        	GLFW_KEY_SPACE
#define KEY_APOSTROPHE                    	GLFW_KEY_APOSTROPHE    /* ' */
#define KEY_COMMA                        	GLFW_KEY_COMMA         /* , */
#define KEY_MINUS                        	GLFW_KEY_MINUS         /* - */
#define KEY_PERIOD                        	GLFW_KEY_PERIOD        /* . */
#define KEY_SLASH                        	GLFW_KEY_SLASH         /* / */
#define KEY_0                            	GLFW_KEY_0
#define KEY_1                            	GLFW_KEY_1
#define KEY_2                            	GLFW_KEY_2
#define KEY_3                            	GLFW_KEY_3
#define KEY_4                            	GLFW_KEY_4
#define KEY_5                            	GLFW_KEY_5
#define KEY_6                            	GLFW_KEY_6
#define KEY_7                            	GLFW_KEY_7
#define KEY_8                            	GLFW_KEY_8
#define KEY_9                            	GLFW_KEY_9
#define KEY_SEMICOLON                    	GLFW_KEY_SEMICOLON     /* ; */
#define KEY_EQUAL                        	GLFW_KEY_EQUAL         /* = */
#define KEY_A                            	GLFW_KEY_A
#define KEY_B                            	GLFW_KEY_B
#define KEY_C                            	GLFW_KEY_C
#define KEY_D                            	GLFW_KEY_D
#define KEY_E                            	GLFW_KEY_E
#define KEY_F                            	GLFW_KEY_F
#define KEY_G                            	GLFW_KEY_G
#define KEY_H                            	GLFW_KEY_H
#define KEY_I                            	GLFW_KEY_I
#define KEY_J                            	GLFW_KEY_J
#define KEY_K                            	GLFW_KEY_K
#define KEY_L                            	GLFW_KEY_L
#define KEY_M                            	GLFW_KEY_M
#define KEY_N                            	GLFW_KEY_N
#define KEY_O                            	GLFW_KEY_O
#define KEY_P                            	GLFW_KEY_P
#define KEY_Q                            	GLFW_KEY_Q
#define KEY_R                            	GLFW_KEY_R
#define KEY_S                            	GLFW_KEY_S
#define KEY_T                            	GLFW_KEY_T
#define KEY_U                            	GLFW_KEY_U
#define KEY_V                            	GLFW_KEY_V
#define KEY_W                            	GLFW_KEY_W
#define KEY_X                            	GLFW_KEY_X
#define KEY_Y                            	GLFW_KEY_Y
#define KEY_Z                            	GLFW_KEY_Z
#define KEY_LEFT_BRACKET                	GLFW_KEY_LEFT_BRACKET  /* [ */
#define KEY_BACKSLASH                    	GLFW_KEY_BACKSLASH     /* \ */
#define KEY_RIGHT_BRACKET                	GLFW_KEY_RIGHT_BRACKET /* ] */
#define KEY_GRAVE_ACCENT                	GLFW_KEY_GRAVE_ACCENT  /* ` */
#define KEY_WORLD_1                        	GLFW_KEY_WORLD_1       /* non-US #1 */
#define KEY_WORLD_2                        	GLFW_KEY_WORLD_2       /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE                        	GLFW_KEY_ESCAPE
#define KEY_ENTER                        	GLFW_KEY_ENTER
#define KEY_TAB                            	GLFW_KEY_TAB
#define KEY_BACKSPACE                    	GLFW_KEY_BACKSPACE
#define KEY_INSERT                        	GLFW_KEY_INSERT
#define KEY_DELETE                        	GLFW_KEY_DELETE
#define KEY_RIGHT                        	GLFW_KEY_RIGHT
#define KEY_LEFT                        	GLFW_KEY_LEFT
#define KEY_DOWN                        	GLFW_KEY_DOWN
#define KEY_UP                            	GLFW_KEY_UP
#define KEY_PAGE_UP                        	GLFW_KEY_PAGE_UP
#define KEY_PAGE_DOWN                    	GLFW_KEY_PAGE_DOWN
#define KEY_HOME                        	GLFW_KEY_HOME
#define KEY_END                            	GLFW_KEY_END
#define KEY_CAPS_LOCK                    	GLFW_KEY_CAPS_LOCK
#define KEY_SCROLL_LOCK                    	GLFW_KEY_SCROLL_LOCK
#define KEY_NUM_LOCK                    	GLFW_KEY_NUM_LOCK
#define KEY_PRINT_SCREEN                	GLFW_KEY_PRINT_SCREEN
#define KEY_PAUSE                        	GLFW_KEY_PAUSE
#define KEY_F1                            	GLFW_KEY_F1
#define KEY_F2                            	GLFW_KEY_F2
#define KEY_F3                            	GLFW_KEY_F3
#define KEY_F4                            	GLFW_KEY_F4
#define KEY_F5                            	GLFW_KEY_F5
#define KEY_F6                            	GLFW_KEY_F6
#define KEY_F7                            	GLFW_KEY_F7
#define KEY_F8                            	GLFW_KEY_F8
#define KEY_F9                            	GLFW_KEY_F9
#define KEY_F10                            	GLFW_KEY_F10
#define KEY_F11                            	GLFW_KEY_F11
#define KEY_F12                            	GLFW_KEY_F12
#define KEY_F13                            	GLFW_KEY_F13
#define KEY_F14                            	GLFW_KEY_F14
#define KEY_F15                            	GLFW_KEY_F15
#define KEY_F16                            	GLFW_KEY_F16
#define KEY_F17                            	GLFW_KEY_F17
#define KEY_F18                            	GLFW_KEY_F18
#define KEY_F19                            	GLFW_KEY_F19
#define KEY_F20                            	GLFW_KEY_F20
#define KEY_F21                            	GLFW_KEY_F21
#define KEY_F22                            	GLFW_KEY_F22
#define KEY_F23                            	GLFW_KEY_F23
#define KEY_F24                            	GLFW_KEY_F24
#define KEY_F25                            	GLFW_KEY_F25
#define KEY_KP_0                        	GLFW_KEY_KP_0
#define KEY_KP_1                        	GLFW_KEY_KP_1
#define KEY_KP_2                        	GLFW_KEY_KP_2
#define KEY_KP_3                        	GLFW_KEY_KP_3
#define KEY_KP_4                        	GLFW_KEY_KP_4
#define KEY_KP_5                        	GLFW_KEY_KP_5
#define KEY_KP_6                        	GLFW_KEY_KP_6
#define KEY_KP_7                        	GLFW_KEY_KP_7
#define KEY_KP_8                        	GLFW_KEY_KP_8
#define KEY_KP_9                        	GLFW_KEY_KP_9
#define KEY_KP_DECIMAL                    	GLFW_KEY_KP_DECIMAL
#define KEY_KP_DIVIDE                    	GLFW_KEY_KP_DIVIDE
#define KEY_KP_MULTIPLY                    	GLFW_KEY_KP_MULTIPLY
#define KEY_KP_SUBTRACT                    	GLFW_KEY_KP_SUBTRACT
#define KEY_KP_ADD                        	GLFW_KEY_KP_ADD
#define KEY_KP_ENTER                    	GLFW_KEY_KP_ENTER
#define KEY_KP_EQUAL                    	GLFW_KEY_KP_EQUAL
#define KEY_LEFT_SHIFT                    	GLFW_KEY_LEFT_SHIFT
#define KEY_LEFT_CONTROL                	GLFW_KEY_LEFT_CONTROL
#define KEY_LEFT_ALT                    	GLFW_KEY_LEFT_ALT
#define KEY_LEFT_SUPER                    	GLFW_KEY_LEFT_SUPER
#define KEY_RIGHT_SHIFT                    	GLFW_KEY_RIGHT_SHIFT
#define KEY_RIGHT_CONTROL                	GLFW_KEY_RIGHT_CONTROL
#define KEY_RIGHT_ALT                    	GLFW_KEY_RIGHT_ALT
#define KEY_RIGHT_SUPER                    	GLFW_KEY_RIGHT_SUPER
#define KEY_MENU                        	GLFW_KEY_MENU

#define KEY_LAST                        	GLFW_KEY_MENU


#define _MOD_SHIFT                    		GLFW_MOD_SHIFT
#define _MOD_CONTROL                    	GLFW_MOD_CONTROL
#define _MOD_ALT                        	GLFW_MOD_ALT
#define _MOD_SUPER                    		GLFW_MOD_SUPER
#define _MOD_CAPS_LOCK                		GLFW_MOD_CAPS_LOCK
#define _MOD_NUM_LOCK                    	GLFW_MOD_NUM_LOCK

#define MOUSE_BUTTON_1                    	GLFW_MOUSE_BUTTON_1
#define MOUSE_BUTTON_2                    	GLFW_MOUSE_BUTTON_2
#define MOUSE_BUTTON_3                    	GLFW_MOUSE_BUTTON_3
#define MOUSE_BUTTON_4                    	GLFW_MOUSE_BUTTON_4
#define MOUSE_BUTTON_5                    	GLFW_MOUSE_BUTTON_5
#define MOUSE_BUTTON_6                    	GLFW_MOUSE_BUTTON_6
#define MOUSE_BUTTON_7                    	GLFW_MOUSE_BUTTON_7
#define MOUSE_BUTTON_8                    	GLFW_MOUSE_BUTTON_8
#define MOUSE_BUTTON_LAST                	GLFW_MOUSE_BUTTON_LAST
#define MOUSE_BUTTON_LEFT                	GLFW_MOUSE_BUTTON_LEFT
#define MOUSE_BUTTON_RIGHT                	GLFW_MOUSE_BUTTON_RIGHT
#define MOUSE_BUTTON_MIDDLE                	GLFW_MOUSE_BUTTON_MIDDLE

#define JOYSTICK_1                        	GLFW_JOYSTICK_1
#define JOYSTICK_2                        	GLFW_JOYSTICK_2
#define JOYSTICK_3                        	GLFW_JOYSTICK_3
#define JOYSTICK_4                        	GLFW_JOYSTICK_4
#define JOYSTICK_5                        	GLFW_JOYSTICK_5
#define JOYSTICK_6                        	GLFW_JOYSTICK_6
#define JOYSTICK_7                        	GLFW_JOYSTICK_7
#define JOYSTICK_8                        	GLFW_JOYSTICK_8
#define JOYSTICK_9                        	GLFW_JOYSTICK_9
#define JOYSTICK_10                        	GLFW_JOYSTICK_10
#define JOYSTICK_11                        	GLFW_JOYSTICK_11
#define JOYSTICK_12                        	GLFW_JOYSTICK_12
#define JOYSTICK_13                        	GLFW_JOYSTICK_13
#define JOYSTICK_14                        	GLFW_JOYSTICK_14
#define JOYSTICK_15                        	GLFW_JOYSTICK_15
#define JOYSTICK_16                        	GLFW_JOYSTICK_16
#define JOYSTICK_LAST                    	GLFW_JOYSTICK_LAST

#define GAMEPAD_BUTTON_A                	GLFW_GAMEPAD_BUTTON_A
#define GAMEPAD_BUTTON_B                	GLFW_GAMEPAD_BUTTON_B
#define GAMEPAD_BUTTON_X                	GLFW_GAMEPAD_BUTTON_X
#define GAMEPAD_BUTTON_Y                	GLFW_GAMEPAD_BUTTON_Y
#define GAMEPAD_BUTTON_LEFT_BUMPER        	GLFW_GAMEPAD_BUTTON_LEFT_BUMPER
#define GAMEPAD_BUTTON_RIGHT_BUMPER        	GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER
#define GAMEPAD_BUTTON_BACK                	GLFW_GAMEPAD_BUTTON_BACK
#define GAMEPAD_BUTTON_START            	GLFW_GAMEPAD_BUTTON_START
#define GAMEPAD_BUTTON_GUIDE            	GLFW_GAMEPAD_BUTTON_GUIDE
#define GAMEPAD_BUTTON_LEFT_THUMB        	GLFW_GAMEPAD_BUTTON_LEFT_THUMB
#define GAMEPAD_BUTTON_RIGHT_THUMB        	GLFW_GAMEPAD_BUTTON_RIGHT_THUMB
#define GAMEPAD_BUTTON_DPAD_UP            	GLFW_GAMEPAD_BUTTON_DPAD_UP
#define GAMEPAD_BUTTON_DPAD_RIGHT        	GLFW_GAMEPAD_BUTTON_DPAD_RIGHT
#define GAMEPAD_BUTTON_DPAD_DOWN        	GLFW_GAMEPAD_BUTTON_DPAD_DOWN
#define GAMEPAD_BUTTON_DPAD_LEFT        	GLFW_GAMEPAD_BUTTON_DPAD_LEFT
#define GAMEPAD_BUTTON_LAST                	GLFW_GAMEPAD_BUTTON_LAST

#define GAMEPAD_BUTTON_CROSS            	GLFW_GAMEPAD_BUTTON_CROSS
#define GAMEPAD_BUTTON_CIRCLE            	GLFW_GAMEPAD_BUTTON_CIRCLE
#define GAMEPAD_BUTTON_SQUARE            	GLFW_GAMEPAD_BUTTON_SQUARE
#define GAMEPAD_BUTTON_TRIANGLE            	GLFW_GAMEPAD_BUTTON_TRIANGLE

#define GAMEPAD_AXIS_LEFT_X                	GLFW_GAMEPAD_AXIS_LEFT_X
#define GAMEPAD_AXIS_LEFT_Y                	GLFW_GAMEPAD_AXIS_LEFT_Y
#define GAMEPAD_AXIS_RIGHT_X				GLFW_GAMEPAD_AXIS_RIGHT_X
#define GAMEPAD_AXIS_RIGHT_Y              	GLFW_GAMEPAD_AXIS_RIGHT_Y
#define GAMEPAD_AXIS_LEFT_TRIGGER         	GLFW_GAMEPAD_AXIS_LEFT_TRIGGER
#define GAMEPAD_AXIS_RIGHT_TRIGGER        	GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
#define GAMEPAD_AXIS_LAST                 	GLFW_GAMEPAD_AXIS_LAST
//@formatter:on


namespace mage
{
	class Input
	{
	public:
		mage3d_EXPORT Input() :
				m_mouseX(0),
				m_mouseY(0),
				m_lastMouseX(0),
				m_lastMouseY(0),
				m_scrollX(0),
				m_scrollY(0){ }

		mage3d_EXPORT ~Input() = default;
		mage3d_EXPORT void onKeyCallback(int key, int scancode, int action, int mods);
		mage3d_EXPORT void onMouseButtonCallback(int button, int action, int mods);

		mage3d_EXPORT void update();

		mage3d_EXPORT bool keyDown(int key);
		mage3d_EXPORT bool keyPressed(int key);
		mage3d_EXPORT bool keyReleased(int key);

		mage3d_EXPORT bool mouseButtonDown(int button);
		mage3d_EXPORT bool mouseButtonPressed(int button);
		mage3d_EXPORT bool mouseButtonReleased(int button);

		mage3d_EXPORT void setMousePos(double x, double y);

		mage3d_EXPORT double getMouseX() const { return m_mouseX; }

		mage3d_EXPORT double getMouseY() const { return m_mouseY; }

		mage3d_EXPORT double getMouseOffsetX() const { return m_mouseX - m_lastMouseX; }

		mage3d_EXPORT double getMouseOffsetY() const { return m_lastMouseY - m_mouseY; }

		mage3d_EXPORT void setMouseScroll(double x, double y);

		mage3d_EXPORT double getScrollX() const { return m_scrollX; }

		mage3d_EXPORT double getScrollY() const { return m_scrollY; }

	protected:
	private:
		bool m_keys[MAX_KEYS] = { false };
		bool m_lastKeys[MAX_KEYS] = { false };

		bool m_mouseButtons[MAX_BUTTONS] = { false };
		bool m_lastMouseButtons[MAX_BUTTONS] = { false };

		double m_mouseX;
		double m_mouseY;
		double m_lastMouseX;
		double m_lastMouseY;

		double m_scrollX;
		double m_scrollY;
	};

}
#endif //MAGE3D_INPUT_H
