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
 * File Name: Cylinder.cpp
 * Date File Created: 9/30/2020 at 4:08 PM
 * Author: Matt
 */
#include "pch.h"
#include "Cylinder.h"
#include "Primitives.h"
#include "Bindables.h"

mage::Cylinder::Cylinder(Graphics& gfx, std::mt19937& rng,
                         std::uniform_real_distribution<float>& adist,
                         std::uniform_real_distribution<float>& ddist,
                         std::uniform_real_distribution<float>& odist,
                         std::uniform_real_distribution<float>& rdist,
                         std::uniform_real_distribution<float>& bdist,
                         std::uniform_int_distribution<int>& tdist) :
	DummyObject(gfx, rng, adist, ddist, odist, rdist)
{
	if (!isInitialized())
	{

		auto pvs = createScope<VertexShader>(gfx, L"shaders\\phongVS.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(createScope<PixelShader>(gfx, L"shaders\\indexedPhongPS.cso"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		addStaticBind(createScope<InputLayout>(gfx, ied, pvsbc));

		addStaticBind(createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			dx::XMFLOAT3A colors[6] = {
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,1.0f,0.0f },
				{ 1.0f,0.0f,1.0f },
				{ 0.0f,1.0f,1.0f },
			};
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
		} matConst;
		addStaticBind(createScope<PixelConstantBuffer<PSMaterialConstant> >(gfx, matConst, 1u));
	}
	
	struct Vertex
	{
		vec3f pos;
		vec3f n;
	};
	auto model = Prism::makeTesselatedNormalsNoCaps<Vertex>(tdist(rng));

	addBind(createScope<VertexBuffer>(gfx, model.vertices));
	addIndexBuffer(createScope<IndexBuffer>(gfx, model.indices));

	addBind(createScope<TransformConstantBuffer>(gfx, *this));
}
