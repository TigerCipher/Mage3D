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

#pragma once

#include <mage3d_exported.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Display
{
public:

	mage3d_EXPORT Display(const char* title, int width, int height);
	mage3d_EXPORT ~Display();

	// Display should never be created with no arguments
	Display() = delete;

	mage3d_EXPORT void update() const;
	mage3d_EXPORT void clear() const;
	mage3d_EXPORT void clear(int red, int green, int blue) const;
	mage3d_EXPORT bool isClosed() const;


	mage3d_EXPORT int getWidth() const { return m_width; }
	mage3d_EXPORT int getHeight() const { return m_height; }
private:

	const char* m_pTitle;
	int m_width, m_height;
	GLFWwindow* m_pWindow;

	int init();

	static friend void windowResize(GLFWwindow* pWindow, int width, int height);
	static friend void errorCallback(int error, const char* desc);
};
