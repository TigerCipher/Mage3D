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

#include "mage/core/display.h"
#include "mage/core/stdcolor.h"
#include <iostream>

#define FULLSCREEN_MODE 0
#define VSYNC 0

mage::Display::Display(const char* title, int width, int height, mage::Input* input) :
		m_pTitle(title),
		m_width(width),
		m_height(height),
		m_pWindow(nullptr),
		m_input(input)
{
	if (!init())
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}
}

mage::Display::~Display()
{
	println(console::BRIGHT_MAGENTA, "Closing display");
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void mage::Display::update() const
{
	const auto error = glGetError();
	if (error != GL_NO_ERROR)
		//std::cout << "OpenGL Error: " << error << ": " << glGetString(error) << std::endl;
		print(console::RED, "OpenGL Error: {}\n", error);

	glfwSwapBuffers(m_pWindow);
	glfwPollEvents();
}

void mage::Display::clear() const
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void mage::Display::clear(int red, int green, int blue) const
{
	glClearColor(red, green, blue, 255);
	clear();
}

bool mage::Display::isClosed() const
{
	return glfwWindowShouldClose(m_pWindow);
}


void mage::windowResize(GLFWwindow* pWindow, int width, int height)
{
	auto* const display = static_cast<mage::Display*>(glfwGetWindowUserPointer(pWindow));
	display->m_width = width;
	display->m_height = height;
	glViewport(0, 0, width, height);
}


void mage::errorCallback(int error, const char* desc)
{
	print(console::RED, "Error ({}):\n{}\n", error, desc);
}

void mage::key_callback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
	auto* display = (mage::Display*)glfwGetWindowUserPointer(pWindow);
	display->m_input->onKeyCallback(key, scancode, action, mods);
}

void mage::mouse_button_callback(GLFWwindow* pWindow, int button, int action, int mods)
{
	auto* display = (mage::Display*)glfwGetWindowUserPointer(pWindow);
	display->m_input->onMouseButtonCallback(button, action, mods);
}

void mage::cursor_position_callback(GLFWwindow* pWindow, double xpos, double ypos)
{
	//if(glfwGetInputMode(pWindow, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) return;
	auto* display = (mage::Display*)glfwGetWindowUserPointer(pWindow);
	display->m_input->setMousePos(xpos, ypos);
}

void mage::scroll_callback(GLFWwindow* pWindow, double xoffset, double yoffset)
{
	auto* display = (mage::Display*)glfwGetWindowUserPointer(pWindow);
	display->m_input->setMouseScroll(xoffset, yoffset);
}

int mage::Display::init()
{
	println(console::BRIGHT_CYAN, "Initializing glfw");
	const auto glfwError = glfwInit();
	if (!glfwError) return glfwError;
	println(console::BRIGHT_CYAN, "glfw initialized\nCreating window");
	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x AA
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL 4+
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL 4.3+
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // I think macos needs this?
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Ew to old opengl
	m_pWindow = glfwCreateWindow(m_width, m_height, m_pTitle, nullptr, nullptr);

	// Start the windowed screen at the center of the primary monitor
	int centerX = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width / 2) - (m_width / 2);
	int centerY = (glfwGetVideoMode(glfwGetPrimaryMonitor())->height / 2) - (m_height / 2);
	glfwSetWindowPos(m_pWindow, centerX, centerY);

	if (!m_pWindow) return -1;
	println(console::BRIGHT_CYAN, "Window created at position ({}, {}) with dimensions ({}, {})", centerX,
			centerY, m_width, m_height);
	//TODO: Set up callbacks and whatnot
	glfwMakeContextCurrent(m_pWindow);
	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetErrorCallback(errorCallback);
	glfwSetKeyCallback(m_pWindow, key_callback);
	glfwSetMouseButtonCallback(m_pWindow, mouse_button_callback);
	glfwSetCursorPosCallback(m_pWindow, cursor_position_callback);
	glfwSetScrollCallback(m_pWindow, scroll_callback);
	glfwSetWindowSizeCallback(m_pWindow, windowResize);

	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if(glfwRawMouseMotionSupported())
		glfwSetInputMode(m_pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glfwSetCursorPos(m_pWindow, 0, 0);


	// VSYNC must be either 1 or 0
	glfwSwapInterval(VSYNC);

	println(console::BRIGHT_CYAN, "Initializing glew");
	const int glError = glewInit();
	if (glError != GLEW_OK) return glError;
	println(console::BRIGHT_CYAN, "Glew initialized");

	print(console::YELLOW, "OpenGL {}\n", glGetString(GL_VERSION));
	return 1;
}

void mage::Display::setTitle(const char* title) const
{
	glfwSetWindowTitle(m_pWindow, title);
}

void mage::Display::toggleCursor() const
{
	int mode = glfwGetInputMode(m_pWindow, GLFW_CURSOR);
	if(mode == GLFW_CURSOR_DISABLED)
	{
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		double x, y;
		glfwGetCursorPos(m_pWindow, &x, &y);
		m_input->setMousePos(x, y);
	}
	else
	{
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(m_pWindow, m_input->getMouseX(), m_input->getMouseY());
	}
}

bool mage::Display::isCursorLocked() const
{
	return glfwGetInputMode(m_pWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}






