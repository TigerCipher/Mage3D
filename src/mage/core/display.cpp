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
// File Name: display.cpp
// Date File Created: 07/09/2020 at 2:17 AM
// Author: Matt / TigerCipher

#include <GL/glew.h>
#include <mage/core/display.h>
#include <iostream>

#define FULLSCREEN_MODE 0
#define SHOW_MOUSE 1
#define VSYNC 0

Display::Display(const char* title, int width, int height) :
	m_pTitle(title),
	m_width(width),
	m_height(height),
	m_pWindow(nullptr)
{
	if (!init())
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}
}

Display::~Display()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void Display::update() const
{
	const auto error = glGetError();
	if (error != GL_NO_ERROR)
		std::cout << "OpenGL Error: " << glGetString(error) << std::endl;

	glfwSwapBuffers(m_pWindow);
	glfwPollEvents();
}

void Display::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::clear(int red, int green, int blue) const
{
	glClearColor(red, green, blue, 255);
	clear();
}

bool Display::isClosed() const
{
	return glfwWindowShouldClose(m_pWindow);
}



static void windowResize(GLFWwindow* pWindow, int width, int height)
{
	auto* const display = static_cast<Display*>(glfwGetWindowUserPointer(pWindow));
	display->m_width = width;
	display->m_height = height;
	glViewport(0, 0, width, height);
}


static void errorCallback(int error, const char* desc)
{
	fprintf(stderr, "Error (%i):\n%s\n", error, desc);
}

int Display::init()
{
	const auto glfwError = glfwInit();
	if (!glfwError) return glfwError;

	m_pWindow = glfwCreateWindow(m_width, m_height, m_pTitle, nullptr, nullptr);

	if (!m_pWindow) return -1;

	//TODO: Set up callbacks and whatnot
	glfwMakeContextCurrent(m_pWindow);
	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetErrorCallback(errorCallback);
	glfwSetWindowSizeCallback(m_pWindow, windowResize);

	// VSYNC must be either 1 or 0
	glfwSwapInterval(VSYNC);

	const int glError = glewInit();
	if (glError != GLEW_OK) return glError;

	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

	return 1;
}
