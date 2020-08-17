// Mage3D
// Copyright (C) 2020 Blue Moon Development
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// 
// Contact: team@bluemoondev.org
// 
// File Name: display.h
// Date File Created: 07/09/2020 at 2:14 AM
// Author: Matt / TigerCipher

#ifndef MAGE3D_DISPLAY_H
#define MAGE3D_DISPLAY_H

#include "mage3d_exported.h"
#include "mage/common.h"
#include "input.h"

namespace mage
{
	class Display
	{
	public:

		mage3d_EXPORT static void create(const char* title, int width, int height, Input* input);
		mage3d_EXPORT static void destroy();

		mage3d_EXPORT static void update();
		mage3d_EXPORT static void clear();
		mage3d_EXPORT static void clear(int red, int green, int blue);
		mage3d_EXPORT static bool isClosed();
		mage3d_EXPORT static void setTitle(const char* title);
		mage3d_EXPORT static void toggleCursor();
		mage3d_EXPORT static bool isCursorLocked();

		mage3d_EXPORT static int getWidth() { return m_width; }

		mage3d_EXPORT static int getHeight() { return m_height; }

	private:

		static const char* m_pTitle;
		static int m_width, m_height;
		static GLFWwindow* m_pWindow;
		static Input* m_input;

		static int init();

		friend void windowResize(GLFWwindow* pWindow, int width, int height);
		friend void errorCallback(int error, const char* desc);
		friend void key_callback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
		friend void mouse_button_callback(GLFWwindow* pWindow, int button, int action, int mods);
		friend void cursor_position_callback(GLFWwindow* pWindow, double xpos, double ypos);
		friend void scroll_callback(GLFWwindow* pWindow, double xoffset, double yoffset);
	};
}

#endif //MAGE3D_DISPLAY_H