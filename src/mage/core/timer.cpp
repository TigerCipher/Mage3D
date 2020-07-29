
#include "mage/core/timer.h"

#include <GLFW/glfw3.h>

mage::Timer::Timer() :
		m_time(glfwGetTime()),
		m_lastTime(0)
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

void mage::Timer::update()
{
	double current = currentTime();
	m_delta = (float) (current - m_lastTime);
	m_lastTime = current;
}