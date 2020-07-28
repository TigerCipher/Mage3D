/*
 * Mage3D
 * Copyright (C) 2020 Blue Moon Development. All rights reserved.
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: team@bluemoondev.org
 * 
 * File Name: input.cpp
 * Date File Created: 7/27/2020 at 4:38 PM
 * Author: Matt
 */

#include "mage/core/input.h"
#include "mage/core/stdcolor.h"

void mage::Input::onKeyCallback(int key, int scancode, int action, int mods)
{
	m_keys[key] = action != GLFW_RELEASE;
}

void mage::Input::onMouseButtonCallback(int button, int action, int mods)
{
	m_mouseButtons[button] = action != GLFW_RELEASE;
}

bool mage::Input::keyDown(int key)
{
	return m_keys[ key ];
}

bool mage::Input::keyPressed(int key)
{
	return m_keys[ key ] && !m_lastKeys[ key ];
}

bool mage::Input::keyReleased(int key)
{
	return !m_keys[ key ] && m_lastKeys[ key ];
}

bool mage::Input::mouseButtonDown(int button)
{
	return m_mouseButtons[button];
}

bool mage::Input::mouseButtonPressed(int button)
{
	return m_mouseButtons[button] && !m_lastMouseButtons[button];
}

bool mage::Input::mouseButtonReleased(int button)
{
	return !m_mouseButtons[button] && m_lastMouseButtons[button];
}

void mage::Input::update()
{
	for(int i = 0; i < MAX_KEYS; i++)
	{
		m_lastKeys[i] = m_keys[i];
	}

	for(int i = 0; i < MAX_BUTTONS; i++)
	{
		m_lastMouseButtons[i] = m_mouseButtons[i];
	}

	m_lastMouseX = m_mouseX;
	m_lastMouseY = m_mouseY;
}

void mage::Input::setMousePos(double x, double y)
{
	m_mouseX = x;
	m_mouseY = y;
}

void mage::Input::setMouseScroll(double x, double y)
{
	m_scrollX = x;
	m_scrollY = y;
}




