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
#include <GLFW/glfw3.h>

#define MAX_KEYS 1024
#define MAX_BUTTONS 32

namespace mage
{
	class Input
	{
	public:
		mage3d_EXPORT Input():m_mouseX(0), m_mouseY(0), m_lastMouseX(0), m_lastMouseY(0) {}
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
		bool m_keys[MAX_KEYS] = { 0};
		bool m_lastKeys[MAX_KEYS] = { 0};

		bool m_mouseButtons[MAX_BUTTONS] = { 0};
		bool m_lastMouseButtons[MAX_BUTTONS] = { 0};

		double m_mouseX;
		double m_mouseY;
		double m_lastMouseX;
		double m_lastMouseY;

		double m_scrollX;
		double m_scrollY;
	};

}
#endif //MAGE3D_INPUT_H
