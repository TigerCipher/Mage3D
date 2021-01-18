/*
 * Mage3DX
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
 * File Name: Camera.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "MathHelper.h"


class Camera
{
public:
	Camera() = default;
	virtual ~Camera() = default;

	void spawnControlWindow() noexcept;
	void reset() noexcept;

	[[nodiscard]] mat4f getViewMatrix() const noexcept;


private:
	float mR = 20.0f;
	float mTheta = 0.0f;
	float mPhi = 0.0f;
	float mPitch = 0.0f;
	float mYaw = 0.0f;
	float mRoll = 0.0f;
};

