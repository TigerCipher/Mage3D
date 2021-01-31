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
 * File Name: TestPlane.cpp
 * Date File Created: 1/23/2021 at 5:52 PM
 * Author: Matt
 */
#include "TestPlane.h"
#include "Bindables.h"
#include "Primitives.h"
#include "ImguiManager.h"

TestPlane::TestPlane(Graphics& gfx, const float size, vec4f color) :
	mMatConst({color})
{
	auto model = Plane::make();
	model.transform(scaleMatrix(size, size, 1));
	const auto tag = "@plane." + std::to_string(size);
	addBind(VertexBufferBindable::resolve(gfx, tag, model.vertices));
	addBind(IndexBuffer::resolve(gfx, tag, model.indices));

	auto vs = VertexShader::resolve(gfx, "shaders\\solidVS.cso");
	auto* vsb = vs->getBytecode();
	addBind(std::move(vs));

	addBind(PixelShader::resolve(gfx, "shaders\\solidPS.cso"));

	

	addBind(createRef<PixelConstantBuffer<MaterialConstant>>(gfx, mMatConst, 1));
	addBind(InputLayout::resolve(gfx, model.vertices.getLayout(), vsb));
	addBind(Topology::resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	addBind(createRef<TransformConstantBuffer>(gfx, *this, 0));

	addBind(Blender::resolve(gfx, true, 0.5f));
	addBind(Rasterizer::resolve(gfx, true));
	
}

void TestPlane::setPosition(vec3f pos) noexcept
{
	mPos = pos;
}

void TestPlane::setRotation(float roll, float pitch, float yaw) noexcept
{
	mRoll = roll;
	mPitch = pitch;
	mYaw = yaw;
}

mat4f TestPlane::getTransformMatrix() const noexcept
{
	return rollPitchYaw(mRoll, mPitch, mYaw) * translateMatrix(mPos.x, mPos.y, mPos.z);
}

void TestPlane::spawnControlWindow(Graphics& gfx, const std::string& name) noexcept
{
	IMGUI_BEGIN(name.c_str())
	{
		IMGUI_FUNC(Text("Position"));
		IMGUI_FUNC(SliderFloat("X", &mPos.x, -80.0f, 80.0f, "%.1f"));
		IMGUI_FUNC(SliderFloat("Y", &mPos.y, -80.0f, 80.0f, "%.1f"));
		IMGUI_FUNC(SliderFloat("Z", &mPos.z, -80.0f, 80.0f, "%.1f"));
		IMGUI_FUNC(Text("Orientation"));
		IMGUI_FUNC(SliderAngle("Roll", &mRoll, -180.0f, 180.0f));
		IMGUI_FUNC(SliderAngle("Pitch", &mPitch, -180.0f, 180.0f));
		IMGUI_FUNC(SliderAngle("Yaw", &mYaw, -180.0f, 180.0f));
		IMGUI_FUNC(Text("Shading"));
		auto blender = queryBindable<Blender>();
		float factor = blender->getFactor();
		IMGUI_FUNC(SliderFloat("Translucency", &factor, 0.0f, 1.0f));
		blender->setFactor(factor);
	}
	IMGUI_END
}
