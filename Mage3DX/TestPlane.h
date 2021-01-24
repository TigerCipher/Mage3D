/*
 * Mage3DX
 * Copyright (C) 2021 Blue Moon Development. All rights reserved.
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
 * File Name: TestPlane.h
 * Date File Created: 1/23/2021 at 5:52 PM
 * Author: Matt
 */
#pragma once

#include "IRenderable.h"

class TestPlane : public IRenderable
{
public:
	TestPlane(Graphics& gfx, float size);
	void setPosition(vec3f pos) noexcept;
	void setRotation(float roll, float pitch, float yaw) noexcept;
	mat4f getTransformMatrix() const noexcept override;

	void spawnControlWindow(Graphics& gfx) noexcept;
protected:
private:

	struct MaterialConstant
	{
		float specularIntensity = 0.1f;
		float specularPower = 20.0f;
		BOOL normalMapEnabled = TRUE;
		float padding[1];
	} mMatConst;
	
	vec3f mPos = { 1, 1, 1 };
	float mRoll = 0;
	float mPitch = 0;
	float mYaw = 0;
};
