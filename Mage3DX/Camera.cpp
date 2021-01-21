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
 * File Name: camera.cpp
 * Date File Created: 9/25/2020 at 8:10 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "Camera.h"

#include "ImguiManager.h"


Camera::Camera() noexcept
{
	reset();
}

void Camera::spawnControlWindow() noexcept
{
	IMGUI_BEGIN("Camera")
	ImGui::Text("Position");
	ImGui::SliderFloat("X", &mPosition.x, 0.1f, 80.0f, "%.1f");
	ImGui::SliderAngle("Y", &mPosition.y, -180.0f, 180.0f, "%.1f");
	ImGui::SliderAngle("Z", &mPosition.z, -89.0f, 89.0f, "%.1f");

	ImGui::Text("Orientation");
	ImGui::SliderAngle("Pitch", &mPitch, -180.0f, 180.0f);
	ImGui::SliderAngle("Yaw", &mYaw, -180.0f, 180.0f);
	if (ImGui::Button("Reset")) reset();
	IMGUI_END()
}

void Camera::reset() noexcept
{
	mPosition = { 0, 7.5f, -18.0f };
	mPitch = 0.0f;
	mYaw = 0.0f;
}

void Camera::rotate(float dx, float dy) noexcept
{
	mYaw = wrapAngle(mYaw + dx * mSensitivity);
	mPitch = std::clamp(mPitch + dy * mSensitivity, -PI / 2.0f, PI / 2.0f);
}

void Camera::translate(vec3f translation) noexcept
{
	dx::XMStoreFloat3(&translation, dx::XMVector3Transform(dx::XMLoadFloat3(&translation),
		dx::XMMatrixRotationRollPitchYaw(mPitch, mYaw, 0.0f)
		* dx::XMMatrixScaling(mSpeed, mSpeed, mSpeed)));

	mPosition = {
		mPosition.x + translation.x,
		mPosition.y + translation.y,
		mPosition.z + translation.z
	};
}

mat4f Camera::getViewMatrix() const noexcept
{
	return dx::XMMatrixTranslation(-mPosition.x, -mPosition.y, -mPosition.z)
	       * dx::XMMatrixRotationRollPitchYaw(-mPitch, -mYaw, 0.0f);
}
