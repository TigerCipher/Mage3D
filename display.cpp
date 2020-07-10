#include "display.h"
#include <iostream>

#define FULLSCREEN_MODE 0
#define SHOW_MOUSE 1
#define VSYNC 0

Display::Display(const char* title, int width, int height) :
	m_pTitle(title),
	m_width(width),
	m_height(height)
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

void Display::update()
{
	GLenum error = glGetError();
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
	Display* display = (Display*)glfwGetWindowUserPointer(pWindow);
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
	int glfwError = glfwInit();
	if (!glfwError) return glfwError;

	if (FULLSCREEN_MODE)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		m_width = mode->width;
		m_height = mode->height;
		m_pWindow = glfwCreateWindow(m_width, m_height, m_pTitle, glfwGetPrimaryMonitor(), NULL);
	}else
		m_pWindow = glfwCreateWindow(m_width, m_height, m_pTitle, NULL, NULL);

	if (!m_pWindow) return -1;

	if (!SHOW_MOUSE)
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//TODO: Set up callbacks and whatnot
	glfwMakeContextCurrent(m_pWindow);
	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetErrorCallback(errorCallback);
	glfwSetWindowSizeCallback(m_pWindow, windowResize);

	// VSYNC must be either 1 or 0
	glfwSwapInterval(VSYNC);

	int glError = glewInit();
	if (glError != GLEW_OK) return glError;

	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

	return 1;
}


