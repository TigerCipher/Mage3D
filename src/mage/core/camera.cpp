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
 * File Name: camera.cpp
 * Date File Created: 7/27/2020 at 9:47 PM
 * Author: Matt
 */

#include "mage/core/display.h"
#include "mage/core/camera.h"

mage::Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
		m_position(position),
		m_worldUp(up),
		m_yaw(yaw),
		m_pitch(pitch),
		m_fov(FOV),
		m_sensitivity(SENSITIVITY),
		m_speed(SPEED),
		m_constrainPitch(false),
		m_zNear(Z_NEAR),
		m_zFar(Z_FAR)
{
	updateVectors();
}

void mage::Camera::updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

void mage::Camera::update(mage::Input input, float delta)
{
	float velocity = m_speed * delta;
	if (input.keyDown(KEY_W))
		m_position += m_front * velocity;
	if (input.keyDown(KEY_S))
		m_position -= m_front * velocity;
	if (input.keyDown(KEY_A))
		m_position -= m_right * velocity;
	if (input.keyDown(KEY_D))
		m_position += m_right * velocity;

	float xOff = input.getMouseOffsetX() * m_sensitivity;
	float yOff = input.getMouseOffsetY() * m_sensitivity;

	if (Display::isCursorLocked())
	{
		m_yaw += xOff;
		m_pitch += yOff;
	}

	if (m_constrainPitch)
		clampPitch();
	updateVectors();
}

glm::mat4 mage::Camera::getViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 mage::Camera::getProjectionMatrix()
{
	return glm::perspective(glm::radians(m_fov),
							(float) Display::getWidth() / (float) Display::getHeight(), m_zNear, m_zFar);
}

void mage::Camera::clampPitch()
{
	m_pitch = m_pitch > 89.0f ? 89.0f : (m_pitch < -89.0f ? -89.0f : m_pitch);
}
