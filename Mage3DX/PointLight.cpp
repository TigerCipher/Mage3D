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
//#include "pch.h" -intellisense works better with force include being used
#include "PointLight.h"
#include "ImguiManager.h"

PointLight::PointLight(Graphics& gfx, float radius) :
	mMesh(gfx, radius),
	mBuffer(gfx)
{
	mCbuf = {
		{ 2.0f, 0, -5.0f },
		{ 0.1f, 0.1f, 0.1f },
		{ 1.0f, 1.0f, 1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f
	};
}

void PointLight::render(Graphics& gfx) const noexcept(!MAGE_DEBUG)
{
	mMesh.setPosition(mCbuf.pos);
	mMesh.render(gfx);
}

void PointLight::bind(Graphics& gfx, mat4f view) const noexcept
{
	auto cpy = mCbuf;
	const auto pos = dx::XMLoadFloat3(&mCbuf.pos);
	dx::XMStoreFloat3(&cpy.pos, dx::XMVector3Transform(pos, view));
	mBuffer.update(gfx, cpy);
	mBuffer.bind(gfx);
}

void PointLight::spawnControlWindow() noexcept
{
	IMGUI_BEGIN("Point Light")
	{
		IMGUI_FUNC(Text("Position"));
		IMGUI_FUNC(SliderFloat("X", &mCbuf.pos.x, -60.0f, 60.0f, "%.1f"));
		IMGUI_FUNC(SliderFloat("Y", &mCbuf.pos.y, -60.0f, 60.0f, "%.1f"));
		IMGUI_FUNC(SliderFloat("Z", &mCbuf.pos.z, -60.0f, 60.0f, "%.1f"));

		IMGUI_FUNC(Text("Intensity / Color"));
		IMGUI_FUNC(SliderFloat("Intensity", &mCbuf.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2));
		IMGUI_FUNC(ColorEdit3("Diffuse Color", &mCbuf.diffuseColor.x));
		IMGUI_FUNC(ColorEdit3("Ambient", &mCbuf.ambient.x));

		IMGUI_FUNC(Text("Attenuation"));
		IMGUI_FUNC(SliderFloat("Constant", &mCbuf.attConst, 0.05f, 10.0f, "%.2f", 4));
		IMGUI_FUNC(SliderFloat("Linear", &mCbuf.attLin, 0.0001f, 4.0f, "%.4f", 8));
		IMGUI_FUNC(SliderFloat("Quadradic", &mCbuf.attQuad, 0.0000001f, 10.0f, "%.7f", 10));

		reset();
	}
	IMGUI_END
}

void PointLight::reset() noexcept
{
	IMGUI_FUNC_COND(Button("Reset"))
	{
		mCbuf = {
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
