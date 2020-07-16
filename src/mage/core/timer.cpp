
#include <mage/core/timer.h>

#include <GLFW/glfw3.h>

Timer::Timer():
	m_time(glfwGetTime())
	{}

void Timer::reset()
{
	m_time = glfwGetTime();
}

double Timer::elapsed()
{
	return glfwGetTime() - m_time;
}