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
#include "pch.h"
#include "Camera.h"

#include "ImguiManager.h"


void mage::Camera::spawnControlWindow() noexcept
{
    IMGUI_WRAP("Camera",
               ImGui::Text("Position"),
               ImGui::SliderFloat("R", &m_r, 0.1f, 80.0f, "%.1f"),
               ImGui::SliderAngle("Theta", &m_theta, -180.0f, 180.0f),
               ImGui::SliderAngle("Phi", &m_phi, -89.0f, 89.0f),

               ImGui::Text("Orientation"),
               ImGui::SliderAngle("Roll", &m_roll, -180.0f, 180.0f),
               ImGui::SliderAngle("Pitch", &m_pitch, -180.0f, 180.0f),
               ImGui::SliderAngle("Yaw", &m_yaw, -180.0f, 180.0f),
               reset()
              );

}

void mage::Camera::reset() noexcept
{
    if(ImGui::Button("Reset"))
    {
        m_r = 20.0f;
        m_theta = 0.0f;
        m_phi = 0.0f;
        m_pitch = 0.0f;
        m_yaw = 0.0f;
        m_roll = 0.0f;
    }
}

mat4f mage::Camera::getViewMatrix() const noexcept
{
    const auto pos = dx::XMVector3Transform(dx::XMVectorSet(0, 0, -m_r, 0),
                                            dx::XMMatrixRotationRollPitchYaw(m_phi, -m_theta, 0));
    return dx::XMMatrixLookAtLH(pos, dx::XMVectorZero(), dx::XMVectorSet(0, 1, 0, 0)) *
           dx::XMMatrixRotationRollPitchYaw(m_pitch, -m_yaw, m_roll);
}
