#pragma once

#include <GLFW/glfw3.h>

class Timer
{
public:
	Timer()
	{
		m_time = glfwGetTime();
	}

	void reset()
	{
		m_time = glfwGetTime();
	}

	double elapsed()
	{
		return glfwGetTime() - m_time;
	}
private:
	double m_time;
};