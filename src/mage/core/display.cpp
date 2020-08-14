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

#include "mage/common.h"

#define FULLSCREEN_MODE 0
#define VSYNC 0

const char* mage::Display::m_pTitle;
int mage::Display::m_width, mage::Display::m_height;
GLFWwindow* mage::Display::m_pWindow;
mage::Input* mage::Display::m_input;

void mage::Display::create(const char* title, int width, int height, mage::Input* input)
{
	m_pWindow = nullptr;
	m_input = input;
	m_pTitle = title;
	m_width = width;
	m_height = height;
	if (!init())
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}
}

void mage::Display::destroy()
{
	DBGPRINT("Closing display");
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void mage::Display::update()
{
	//const auto error = glGetError();
	////if (error != GL_NO_ERROR)
	////	fprintf(stderr, "OpenGL Error (%i):\n%s\n", error, glGetString(error));

	glfwSwapBuffers(m_pWindow);
	glfwPollEvents();
}

void mage::Display::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void mage::Display::clear(int red, int green, int blue)
{
	glClearColor(red, green, blue, 255);
	clear();
}

bool mage::Display::isClosed()
{
	return glfwWindowShouldClose(m_pWindow);
}


void mage::windowResize(GLFWwindow* pWindow, int width, int height)
{
	//auto* const display = static_cast<mage::Display*>(glfwGetWindowUserPointer(pWindow));
	Display::m_width = width;
	Display::m_height = height;
	glViewport(0, 0, width, height);
}


void mage::errorCallback(int error, const char* desc)
{
	DBGPRINT_ERR("Error (%i):\n%s", error, desc);
}

void mage::key_callback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
	//auto* display = (mage::Display*)glfwGetWindowUserPointer(pWindow);
	Display::m_input->onKeyCallback(key, scancode, action, mods);
}

void mage::mouse_button_callback(GLFWwindow* pWindow, int button, int action, int mods)
{
	//auto* display = (mage::Display*)glfwGetWindowUserPointer(pWindow);
	Display::m_input->onMouseButtonCallback(button, action, mods);
}

void mage::cursor_position_callback(GLFWwindow* pWindow, double xpos, double ypos)
{
	//if(glfwGetInputMode(pWindow, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) return;
	//auto* display = (mage::Display*)glfwGetWindowUserPointer(pWindow);
	Display::m_input->setMousePos(xpos, ypos);
}

void mage::scroll_callback(GLFWwindow* pWindow, double xoffset, double yoffset)
{
	//auto* display = (mage::Display*)glfwGetWindowUserPointer(pWindow);
	Display::m_input->setMouseScroll(xoffset, yoffset);
}

void
debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
              const void* userParam)
{
	VERBOSE_PRINT_ERR("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s",
	                  (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
	                  type, severity, message);
}

int mage::Display::init()
{
	DBGPRINT("Initializing glfw");
	int glfwStatus = glfwInit();
	if (!glfwStatus)
	{
		DBGPRINT_ERR("Failed to initialize GLFW with error %i", glfwStatus);
		return glfwStatus;
	}
	DBGPRINT("glfw initialized\nCreating window");
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
	DBGPRINT("Window created at position (%i, %i) with dimensions (%i, %i)", centerX,
	         centerY, m_width, m_height);
	//TODO: Set up callbacks and whatnot
	glfwMakeContextCurrent(m_pWindow);
	//glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetErrorCallback(errorCallback);
	glfwSetKeyCallback(m_pWindow, key_callback);
	glfwSetMouseButtonCallback(m_pWindow, mouse_button_callback);
	glfwSetCursorPosCallback(m_pWindow, cursor_position_callback);
	glfwSetScrollCallback(m_pWindow, scroll_callback);
	glfwSetWindowSizeCallback(m_pWindow, windowResize);

	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(m_pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glfwSetCursorPos(m_pWindow, 0, 0);


	// VSYNC must be either 1 or 0
	glfwSwapInterval(VSYNC);

	DBGPRINT("Initializing glew");
	const int glewError = glewInit();
	if (glewError)
	{
		DBGPRINT_ERR("Error while initializing glew. Error: %i", glewError);
		return glewError;
	}
	DBGPRINT("Glew initialized");

	DBGPRINT("OpenGL %s", glGetString(GL_VERSION));
#ifdef MAGE_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, 0);
#endif
	// TODO some models might need different culling
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return 1;
}

void mage::Display::setTitle(const char* title)
{
	glfwSetWindowTitle(m_pWindow, title);
}

void mage::Display::toggleCursor()
{
	int mode = glfwGetInputMode(m_pWindow, GLFW_CURSOR);
	if (mode == GLFW_CURSOR_DISABLED)
	{
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		double x, y;
		glfwGetCursorPos(m_pWindow, &x, &y);
		m_input->setMousePos(x, y);
	} else
	{
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(m_pWindow, m_input->getMouseX(), m_input->getMouseY());
	}
}

bool mage::Display::isCursorLocked()
{
	return glfwGetInputMode(m_pWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}






