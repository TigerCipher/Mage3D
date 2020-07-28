
#include "mage/core/timer.h"

#include <GLFW/glfw3.h>

mage::Timer::Timer() :
m_time(glfwGetTime())
{
}

void mage::Timer::reset()
{
	m_time = glfwGetTime();
}

double mage::Timer::elapsed()
{
	return glfwGetTime() - m_time;
}

double mage::Timer::currentTime()
{
	return glfwGetTime();
}
