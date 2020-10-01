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
 * File Name: box.cpp
 * Date File Created: 9/25/2020 at 11:49 PM
 * Author: Matt
 */


#include "Box.h"
#include "Bindables.h"
#include "Primitives.h"
#include "Sampler.h"
#include "Texture.h"
#include "TextureSurface.h"


mage::Box::Box(mage::Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
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

	addBindable(createScope<TransformConstantBuffer>(gfx, *this));

	struct MaterialConst
	{
		alignas(16) vec3f color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;

		float padding[2];
	} colConst;
	colConst.color = material;
	addBindable(createScope<PixelConstantBuffer<MaterialConst> >(gfx, colConst, 1));

	dx::XMStoreFloat3x3(&mt, dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}


mat4f mage::Box::getTransformMatrix() const noexcept
{
	return dx::XMLoadFloat3x3(&mt) * DummyObject::getTransformMatrix();
}


mage::SkinnedBox::SkinnedBox(mage::Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
                             std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist,
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

		addStaticBind(createScope<PixelConstantBuffer<MaterialConst>>(gfx, colConst, 1));

	} else
	{
		setIndexStatic();
	}

	addBindable(createScope<TransformConstantBuffer>(gfx, *this));
}
