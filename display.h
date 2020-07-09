#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Display
{
public:

	Display(const char* title, int width, int height);
	~Display();

	void update();
	void clear() const;
	bool isClosed() const;


	inline int getWidth() { return m_width; }
	inline int getHeight() { return m_height; }
private:
	const char* m_pTitle;
	int m_width, m_height;
	GLFWwindow* m_pWindow;

	int init();

};