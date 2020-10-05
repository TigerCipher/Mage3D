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
 * File Name: Box.cpp
 * Date File Created: 9/28/2020 at 1:11 PM
 * Author: Matt
 */
#include "pch.h"
#include "Box.h"
#include "Primitives.h"
#include "Sampler.h"
#include "Texture.h"
#include "TextureSurface.h"
#include "ImguiManager.h"


Box::Box(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
               std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist,
               std::uniform_real_distribution<float>& rdist, std::uniform_real_distribution<float>& bdist,
               vec3f material) :
	DummyObject(gfx, rng, adist, ddist, odist, rdist)
{
	if (!isInitialized())
	{
		struct Vertex {
			vec3f pos;
			vec3f normal;
		};
		auto model = Cube::makeIndependent<Vertex>();
		model.setNormalsFlat();
		//model.transform( dx::XMMatrixScaling( 1.0f,1.0f,1.2f ) );

		addStaticBind(createScope<VertexBuffer>(gfx, model.vertices));

		auto pvs = createScope<VertexShader>(gfx, L"shaders\\phongVS.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(createScope<PixelShader>(gfx, L"shaders\\phongPS.cso"));

		addStaticIndexBuffer(createScope<IndexBuffer>(gfx, model.indices));


		const list<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		addStaticBind(createScope<InputLayout>(gfx, ied, pvsbc));

		addStaticBind(createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	} else
	{
		setIndexStatic();
	}

	addBind(createScope<TransformConstantBuffer>(gfx, *this));

	matConstant.color = material;
	addBind(createScope<MaterialConstBuffer>(gfx, matConstant, 1));

	dx::XMStoreFloat3x3(&m_modelTransformation, dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}


mat4f Box::getTransformMatrix() const noexcept
{
	return dx::XMLoadFloat3x3(&m_modelTransformation) * DummyObject::getTransformMatrix();
}

bool Box::spawnControlWindow(int id, Graphics& gfx) noexcept
{
	bool dirty = false;
	bool open = true;
	IMGUI_WRAP_CLOSABLE(("Box " + std::to_string(id)).c_str(), open,
		ImGui::Text("Material"),
		(dirty = ImGui::ColorEdit3("Material Color", &matConstant.color.x) || dirty),
		(dirty = ImGui::SliderFloat("Specular Intensity", &matConstant.intensity, 0.05f, 4.0f, "%.2f", 2) || dirty),
		(dirty = ImGui::SliderFloat("Specular Power", &matConstant.power, 1.0f, 200.0f, "%.2f", 2) || dirty),
		ImGui::Text("Position"),
		ImGui::SliderFloat("R", &r, 0.0f, 80.0f, "%.1f"),
		ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f),
		ImGui::SliderAngle("Phi",   &phi,   -180.0f, 180.0f),
		ImGui::Text("Orientation"),
		ImGui::SliderAngle("Roll",  &roll,  -180.0f, 180.0f),
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f),
		ImGui::SliderAngle("Yaw",   &yaw,   -180.0f, 180.0f)
		);
	if (dirty)
		syncMaterial(gfx);
	return open;
}

void Box::syncMaterial(Graphics& gfx) noexcept(!MAGE_DEBUG)
{
	auto cBuf = queryBindable<MaterialConstBuffer>();
	assert(cBuf != nullptr);
	cBuf->update(gfx, matConstant);
}


SkinnedBox::SkinnedBox(Graphics& gfx, std::mt19937& rng,
                             std::uniform_real_distribution<float>& adist,
                             std::uniform_real_distribution<float>& ddist,
                             std::uniform_real_distribution<float>& odist,
                             std::uniform_real_distribution<float>& rdist) :
	DummyObject(gfx, rng, adist, ddist, odist, rdist)
{
	if (!isInitialized())
	{
		struct Vertex {
			vec3f pos;
			vec3f normal;
			vec2f texCoords;
		};

		auto model = Cube::makeIndependentTextured<Vertex>();
		model.setNormalsFlat();

		addStaticBind(createScope<Texture>(gfx, TextureSurface::loadFromFile("assets\\textures\\metal_box.png")));

		addStaticBind(createScope<VertexBuffer>(gfx, model.vertices));

		addStaticBind(createScope<Sampler>(gfx));

		auto pvs = createScope<VertexShader>(gfx, L"shaders\\texturePhongVS.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(createScope<PixelShader>(gfx, L"shaders\\texturePhongPS.cso"));

		addStaticIndexBuffer(createScope<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		addStaticBind(createScope<InputLayout>(gfx, ied, pvsbc));

		addStaticBind(createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct MaterialConst
		{
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		} colConst;

		addStaticBind(createScope<PixelConstantBuffer<MaterialConst> >(gfx, colConst, 1));
	} else
	{
		setIndexStatic();
	}

	addBind(createScope<TransformConstantBuffer>(gfx, *this));
}
