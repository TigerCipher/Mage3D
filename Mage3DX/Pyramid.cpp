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
 * File Name: Pyramid.cpp
 * Date File Created: 9/28/2020 at 1:11 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "Pyramid.h"
#include "Bindables.h"
#include "Primitives.h"


Pyramid::Pyramid(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
                       std::uniform_real_distribution<float>& ddist,
                       std::uniform_real_distribution<float>& odist,
                       std::uniform_real_distribution<float>& rdist,
                       std::uniform_int_distribution<int>& tdist) : DummyObject(gfx, rng, adist, ddist, odist, rdist)
{
	if (!isInitialized())
	{
		auto pvs = createScope<VertexShader>(gfx, L"shaders\\blendedPhongVS.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(createScope<PixelShader>(gfx, L"shaders\\blendedPhongPS.cso"));

		const list<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, sizeof(float) * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
	}

	struct Vertex
	{
		vec3f pos;
		vec3f normal;
		std::array<char, 4> color;
		char padding;
	};
	const auto tesselation = tdist(rng);
	auto model = Cone::makeTesselatedIndependentFaces<Vertex>(tesselation);

	for (auto& v : model.vertices)
	{
		v.color = { (char) 10, (char) 10, (char) 255 };
	}

	for (int i = 0; i < tesselation; i++)
	{
		model.vertices[i * 3].color = { (char) 255, (char) 20, (char) 20 };
	}

	model.transform(dx::XMMatrixScaling(1.0f, 1.0f, 0.7f));
	model.setNormalsFlat();

	addBind(createScope<VertexBufferBindable>(gfx, model.vertices));

	addIndexBuffer(createScope<IndexBuffer>(gfx, model.indices));

	addBind(createScope<TransformConstantBuffer>(gfx, *this));
}