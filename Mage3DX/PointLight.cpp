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
 * Date File Created: 9/28/2020 at 1:05 AM
 * Author: Matt
 */
#include "pch.h"
#include "PointLight.h"
#include "ImguiManager.h"

PointLight::PointLight(Graphics& gfx, float radius) :
	m_mesh(gfx, radius),
	m_buffer(gfx)
{
	m_cbuf = {
		{ 0, 0, 0 },
		{ 0.05f, 0.05f, 0.05f },
		{ 1.0f, 1.0f, 1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f
	};
}

void PointLight::render(Graphics& gfx) const noexcept(!MAGE_DEBUG)
{
	m_mesh.setPosition(m_cbuf.pos);
	m_mesh.render(gfx);
}

void PointLight::bind(Graphics& gfx, mat4f view) const noexcept
{
	auto cpy = m_cbuf;
	const auto pos = dx::XMLoadFloat3(&m_cbuf.pos);
	dx::XMStoreFloat3(&cpy.pos, dx::XMVector3Transform(pos, view));
	m_buffer.update(gfx, cpy);
	m_buffer.bind(gfx);
}

void PointLight::spawnControlWindow() noexcept
{
	IMGUI_WRAP("Point Light",
		ImGui::Text("Position"),
		ImGui::SliderFloat("X", &m_cbuf.pos.x, -60.0f, 60.0f, "%.1f"),
		ImGui::SliderFloat("Y", &m_cbuf.pos.y, -60.0f, 60.0f, "%.1f"),
		ImGui::SliderFloat("Z", &m_cbuf.pos.z, -60.0f, 60.0f, "%.1f"),
		
		ImGui::Text("Intensity / Color"),
		ImGui::SliderFloat("Intensity", &m_cbuf.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2),
		ImGui::ColorEdit3("Diffuse Color", &m_cbuf.diffuseColor.x),
		ImGui::ColorEdit3("Ambient", &m_cbuf.ambient.x),

		ImGui::Text("Attenuation"),
		ImGui::SliderFloat("Constant", &m_cbuf.attConst, 0.05f, 10.0f, "%.2f", 4),
		ImGui::SliderFloat("Linear", &m_cbuf.attLin, 0.0001f, 4.0f, "%.4f", 8),
		ImGui::SliderFloat("Quadradic", &m_cbuf.attQuad, 0.0000001f, 10.0f, "%.7f", 10),

		reset()
		);
}

void PointLight::reset() noexcept
{
	if (ImGui::Button("Reset"))
	{
		m_cbuf = {
			{ 0, 0, 0 },
			{ 0.05f, 0.05f, 0.05f },
			{ 1.0f, 1.0f, 1.0f },
			1.0f,
			1.0f,
			0.045f,
			0.0075f
		};
	}
}
