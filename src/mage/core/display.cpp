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
        LOG_CRITICAL("Failed to create display");
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }
}

void mage::Display::destroy()
{
    LOG_INFO("Closing display");
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
}

void mage::Display::update()
{
    GLenum error;
    checkGlError(error);
    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();
}

void mage::Display::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

[[maybe_unused]]
void mage::Display::clear(int red, int green, int blue)
{
    glClearColor(red, green, blue, 255);
    clear();
}

bool mage::Display::isClosed()
{
    return glfwWindowShouldClose(m_pWindow);
}


void mage::windowResize([[maybe_unused]] GLFWwindow* pWindow, int width, int height)
{
    //auto* const display = static_cast<mage::Display*>(glfwGetWindowUserPointer(pWindow));
    Display::m_width = width;
    Display::m_height = height;
    //glViewport(0, 0, width, height);
}


void mage::errorCallback(int error, const char* desc)
{
    LOG_ERROR("Error ({}): {}", error, desc);
}

void mage::key_callback([[maybe_unused]] GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    //auto* display = (mage::Display*)glfwGetWindowUserPointer(pWindow);
    Display::m_input->onKeyCallback(key, scancode, action, mods);
}

void mage::mouse_button_callback([[maybe_unused]] GLFWwindow* pWindow, int button, int action, int mods)
{
    //auto* display = (mage::Display*)glfwGetWindowUserPointer(pWindow);
    Display::m_input->onMouseButtonCallback(button, action, mods);
}

void mage::cursor_position_callback([[maybe_unused]] GLFWwindow* pWindow, double xpos, double ypos)
{
    //if(glfwGetInputMode(pWindow, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) return;
    //auto* display = (mage::Display*)glfwGetWindowUserPointer(pWindow);
    Display::m_input->setMousePos(xpos, ypos);
}

void mage::scroll_callback([[maybe_unused]] GLFWwindow* pWindow, double xoffset, double yoffset)
{
    //auto* display = (mage::Display*)glfwGetWindowUserPointer(pWindow);
    Display::m_input->setMouseScroll(xoffset, yoffset);
}

void
debugCallback([[maybe_unused]] GLenum source, [[maybe_unused]] GLenum type, [[maybe_unused]] GLuint id,
              [[maybe_unused]] GLenum severity, [[maybe_unused]] GLsizei length,
              [[maybe_unused]] const GLchar* message,
              [[maybe_unused]] const void* userParam)
{
    VERBOSE_PRINT_ERR("GL CALLBACK: %s type = 0x%x, severity = 0x%x, id = %i, source = 0x%x, message = %s",
                      (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                      type, severity, id, source, message);
}

int mage::Display::init()
{
    LOG_TRACE("Initializing glfw");
    int glfwStatus = glfwInit();
    if (!glfwStatus)
    {
        LOG_ERROR("Failed to initialize GLFW with error {}", glfwStatus);
        return glfwStatus;
    }
    LOG_TRACE("glfw initialized");
    //glfwWindowHint(GLFW_SAMPLES, 4); // 4x AA
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL 4+
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL 4.3+
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // I think macos needs this?
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Ew to old opengl
    LOG_TRACE("Creating window");
    m_pWindow = glfwCreateWindow(m_width, m_height, m_pTitle, nullptr, nullptr);

    // Start the windowed screen at the center of the primary monitor
    int centerX = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width / 2) - (m_width / 2);
    int centerY = (glfwGetVideoMode(glfwGetPrimaryMonitor())->height / 2) - (m_height / 2);
    glfwSetWindowPos(m_pWindow, centerX, centerY);

    if (!m_pWindow) return -1;
    LOG_TRACE("Window created at position ({}, {}) with dimensions ({}, {})", centerX,
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

    LOG_TRACE("Initializing glew");
    const int glewError = glewInit();
    if (glewError)
    {
        LOG_ERROR("Error while initializing glew. Error: {}", glewError);
        return glewError;
    }
    LOG_TRACE("Glew initialized");

    LOG_INFO("OpenGL Version {}", glGetString(GL_VERSION));
#ifdef MAGE_VERBOSE
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debugCallback, 0);
#endif

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






