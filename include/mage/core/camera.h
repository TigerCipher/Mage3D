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
 * File Name: camera.h
 * Date File Created: 7/27/2020 at 9:47 PM
 * Author: Matt
 */

#ifndef MAGE3D_CAMERA_H
#define MAGE3D_CAMERA_H

#include "mage3d_exported.h"
#include "mage/common.h"
#include "gamecomponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define YAW -90.0f
#define PITCH 0.0f
#define SPEED 3.0f
#define SENSITIVITY 0.05f
#define FOV 45.0f
#define Z_NEAR 0.1f
#define Z_FAR 1000.0f

namespace mage
{
	// TODO: Maybe make 2D and 3D variants for future 2D uses?
	class Camera : public GameComponent
	{
	public:
		mage3d_EXPORT explicit Camera(glm::vec3 position = glm::vec3(0, 0, 0),
									  glm::vec3 up = glm::vec3(0, 1, 0), float yaw = YAW,
									  float pitch = PITCH);

		mage3d_EXPORT void input(Input* _input, float delta) override;

		mage3d_EXPORT glm::mat4 getViewMatrix();
		mage3d_EXPORT glm::mat4 getProjectionMatrix() const;

		inline vec3f getPosition() { return m_position; }

		inline void constrainPitch(bool constrainPitch) { m_constrainPitch = constrainPitch; }

	protected:
	private:

		void updateVectors();
		void clampPitch();

		glm::vec3 m_position;
		glm::vec3 m_front{};
		glm::vec3 m_right{};
		glm::vec3 m_up{};
		glm::vec3 m_worldUp;

		float m_yaw;
		float m_pitch;
		float m_speed;
		float m_sensitivity;
		float m_fov;
		float m_zNear;
		float m_zFar;

		bool m_constrainPitch;
	};

}
#endif //MAGE3D_CAMERA_H
