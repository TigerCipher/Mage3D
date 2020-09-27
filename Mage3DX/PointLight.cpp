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
 * File Name: PointLight.cpp
 * Date File Created: 9/26/2020 at 4:43 PM
 * Author: Matt
 */
#include "PointLight.h"
#include "imgui_manager.h"

void mage::PointLight::render(Graphics& gfx) const noexcept(!MAGE_DEBUG)
{
	m_mesh.setPosition(m_position);
	m_mesh.render(gfx);
}

void mage::PointLight::bind(Graphics& gfx) const noexcept
{
	m_buffer.update(gfx, PointLightCBuf { m_position });
	m_buffer.bind(gfx);
}

void mage::PointLight::spawnControlWindow() noexcept
{ 
	IMGUI_WRAP("Point Light",
			   ImGui::Text("Position"),
			   ImGui::SliderFloat("X", &m_position.x, -60.0f, 60.0f, "%.1f"),
			   ImGui::SliderFloat("Y", &m_position.y, -60.0f, 60.0f, "%.1f"),
			   ImGui::SliderFloat("Z", &m_position.z, -60.0f, 60.0f, "%.1f"),
			   reset()
	);

}

void mage::PointLight::reset() noexcept
{
	if (ImGui::Button("Reset"))
		m_position = { 0, 0, 0 };
}
